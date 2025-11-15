#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static pthread_mutex_t resource_a = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t resource_b = PTHREAD_MUTEX_INITIALIZER;

static void *lock_ab(void *arg);
static void *lock_ba(void *arg);

int main(void) {
  pthread_t t1, t2;

  printf("Starting deadlock demo. Press Ctrl+C to terminate.\n");

  if (pthread_create(&t1, NULL, lock_ab, NULL) != 0) {
    perror("pthread_create");
    return EXIT_FAILURE;
  }
  if (pthread_create(&t2, NULL, lock_ba, NULL) != 0) {
    perror("pthread_create");
    return EXIT_FAILURE;
  }

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  return EXIT_SUCCESS;
}

static void *lock_ab(void *arg) {
  (void)arg;
  while (1) {
    pthread_mutex_lock(&resource_a);
    printf("Thread 1 locked resource A\n");
    sleep(1);
    printf("Thread 1 waiting for resource B\n");
    pthread_mutex_lock(&resource_b);
    printf("Thread 1 acquired both resources (unexpected)\n");
    pthread_mutex_unlock(&resource_b);
    pthread_mutex_unlock(&resource_a);
  }
  return NULL;
}

static void *lock_ba(void *arg) {
  (void)arg;
  while (1) {
    pthread_mutex_lock(&resource_b);
    printf("Thread 2 locked resource B\n");
    sleep(1);
    printf("Thread 2 waiting for resource A\n");
    pthread_mutex_lock(&resource_a);
    printf("Thread 2 acquired both resources (unexpected)\n");
    pthread_mutex_unlock(&resource_a);
    pthread_mutex_unlock(&resource_b);
  }
  return NULL;
}

