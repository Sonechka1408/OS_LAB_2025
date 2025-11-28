#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <stddef.h>

#include "common.h"

struct Server {
  char ip[255];
  int port;
};

struct ServerTask {
  struct Server server;
  uint64_t begin;
  uint64_t end;
  uint64_t mod;
  uint64_t result;
};

int ReadServers(const char* filename, struct Server** servers) {
  FILE* file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Cannot open servers file: %s\n", filename);
    return -1;
  }

  int capacity = 10;
  int count = 0;
  *servers = malloc(sizeof(struct Server) * capacity);

  char line[255];
  while (fgets(line, sizeof(line), file)) {
    // Удаляем символ новой строки
    line[strcspn(line, "\n")] = 0;
    
    if (count >= capacity) {
      capacity *= 2;
      *servers = realloc(*servers, sizeof(struct Server) * capacity);
    }

    char* colon = strchr(line, ':');
    if (colon) {
      *colon = '\0';
      strncpy((*servers)[count].ip, line, sizeof((*servers)[count].ip) - 1);
      (*servers)[count].ip[sizeof((*servers)[count].ip) - 1] = '\0';
      (*servers)[count].port = atoi(colon + 1);
      count++;
    }
  }

  fclose(file);
  return count;
}

void* ProcessServer(void* args) {
  struct ServerTask* task = (struct ServerTask*)args;
  
  struct hostent *hostname = gethostbyname(task->server.ip);
  if (hostname == NULL) {
    fprintf(stderr, "gethostbyname failed with %s\n", task->server.ip);
    task->result = 1;
    return NULL;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(task->server.port);
  
  // Исправление: используем h_addr_list вместо h_addr
  server_addr.sin_addr = *((struct in_addr*)hostname->h_addr_list[0]);

  int sck = socket(AF_INET, SOCK_STREAM, 0);
  if (sck < 0) {
    fprintf(stderr, "Socket creation failed!\n");
    task->result = 1;
    return NULL;
  }

  if (connect(sck, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    fprintf(stderr, "Connection to %s:%d failed\n", task->server.ip, task->server.port);
    close(sck);
    task->result = 1;
    return NULL;
  }

  char task_buffer[sizeof(uint64_t) * 3];
  memcpy(task_buffer, &task->begin, sizeof(uint64_t));
  memcpy(task_buffer + sizeof(uint64_t), &task->end, sizeof(uint64_t));
  memcpy(task_buffer + 2 * sizeof(uint64_t), &task->mod, sizeof(uint64_t));

  if (send(sck, task_buffer, sizeof(task_buffer), 0) < 0) {
    fprintf(stderr, "Send failed to %s:%d\n", task->server.ip, task->server.port);
    close(sck);
    task->result = 1;
    return NULL;
  }

  char response[sizeof(uint64_t)];
  if (recv(sck, response, sizeof(response), 0) < 0) {
    fprintf(stderr, "Receive failed from %s:%d\n", task->server.ip, task->server.port);
    close(sck);
    task->result = 1;
    return NULL;
  }

  memcpy(&task->result, response, sizeof(uint64_t));
  close(sck);
  
  return NULL;
}

int main(int argc, char **argv) {
  uint64_t k = 0;
  uint64_t mod = 0;
  char servers[255] = {'\0'};
  int k_set = 0, mod_set = 0, servers_set = 0;

  while (true) {
    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"servers", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        if (ConvertStringToUI64(optarg, &k)) {
          k_set = 1;
        }
        break;
      case 1:
        if (ConvertStringToUI64(optarg, &mod)) {
          mod_set = 1;
        }
        break;
      case 2:
        memcpy(servers, optarg, strlen(optarg));
        servers_set = 1;
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (!k_set || !mod_set || !servers_set) {
    fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n",
            argv[0]);
    return 1;
  }

  struct Server* server_list = NULL;
  int servers_num = ReadServers(servers, &server_list);
  if (servers_num <= 0) {
    fprintf(stderr, "No servers found or error reading servers file\n");
    return 1;
  }

  pthread_t threads[servers_num];
  struct ServerTask* tasks = malloc(sizeof(struct ServerTask) * servers_num);
  
  uint64_t segment_size = k / servers_num;
  for (int i = 0; i < servers_num; i++) {
    tasks[i].server = server_list[i];
    tasks[i].begin = i * segment_size + 1;
    tasks[i].end = (i == servers_num - 1) ? k : (i + 1) * segment_size;
    tasks[i].mod = mod;
    tasks[i].result = 1;
    
    if (pthread_create(&threads[i], NULL, ProcessServer, (void*)&tasks[i])) {
      fprintf(stderr, "Error creating thread for server %d\n", i);
    }
  }

  uint64_t total_result = 1;
  for (int i = 0; i < servers_num; i++) {
    pthread_join(threads[i], NULL);
    total_result = MultModulo(total_result, tasks[i].result, mod);
  }

  printf("Final answer: %lu\n", total_result);

  free(tasks);
  free(server_list);
  return 0;
}