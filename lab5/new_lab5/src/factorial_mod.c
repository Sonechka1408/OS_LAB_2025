#include <getopt.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct thread_args {
  int begin;
  int end;
  uint64_t mod;
  uint64_t *result;
  pthread_mutex_t *mutex;
};

static void parse_args(int argc, char **argv, int *k, int *pnum, uint64_t *mod);
static void *compute_factorial_segment(void *arg);

int main(int argc, char **argv) {
  int k = -1;
  int pnum = -1;
  uint64_t mod = 0;

  parse_args(argc, argv, &k, &pnum, &mod);

  if (k < 0) {
    fprintf(stderr, "k must be >= 0\n");
    return EXIT_FAILURE;
  }
  if (pnum <= 0) {
    fprintf(stderr, "pnum must be > 0\n");
    return EXIT_FAILURE;
  }
  if (mod == 0) {
    fprintf(stderr, "mod must be > 0\n");
    return EXIT_FAILURE;
  }

  if (pnum > k && k > 0) {
    pnum = k;
  }

  pthread_t *threads = calloc(pnum, sizeof(pthread_t));
  struct thread_args *args = calloc(pnum, sizeof(struct thread_args));
  if (!threads || !args) {
    perror("calloc");
    free(threads);
    free(args);
    return EXIT_FAILURE;
  }

  pthread_mutex_t result_mutex = PTHREAD_MUTEX_INITIALIZER;
  uint64_t factorial = 1 % mod;

  int chunk = (k == 0) ? 0 : k / pnum;
  int remainder = (k == 0) ? 0 : k % pnum;
  int current = 1;

  for (int i = 0; i < pnum; i++) {
    int segment = chunk + (i < remainder ? 1 : 0);
    int begin = current;
    int end = (segment == 0) ? current - 1 : current + segment - 1;
    current = end + 1;

    args[i].begin = begin;
    args[i].end = end;
    args[i].mod = mod;
    args[i].result = &factorial;
    args[i].mutex = &result_mutex;

    if (pthread_create(&threads[i], NULL, compute_factorial_segment,
                       &args[i]) != 0) {
      perror("pthread_create");
      pnum = i;
      goto cleanup_threads;
    }
  }

  for (int i = 0; i < pnum; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      perror("pthread_join");
    }
  }

  printf("%d! mod %" PRIu64 " = %" PRIu64 "\n", k, mod, factorial);

cleanup_threads:
  free(threads);
  free(args);
  return EXIT_SUCCESS;
}

static void parse_args(int argc, char **argv, int *k, int *pnum, uint64_t *mod) {
  while (1) {
    static struct option options[] = {{"k", required_argument, 0, 'k'},
                                      {"pnum", required_argument, 0, 'p'},
                                      {"mod", required_argument, 0, 'm'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "k:p:m:", options, &option_index);
    if (c == -1) {
      break;
    }

    switch (c) {
    case 'k':
      *k = atoi(optarg);
      break;
    case 'p':
      *pnum = atoi(optarg);
      break;
    case 'm':
      *mod = strtoull(optarg, NULL, 10);
      break;
    default:
      fprintf(stderr,
              "Usage: %s -k <value> --pnum=<threads> --mod=<value>\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  if (*k == -1 || *pnum == -1 || *mod == 0) {
    fprintf(stderr,
            "Usage: %s -k <value> --pnum=<threads> --mod=<value>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
}

static void *compute_factorial_segment(void *arg) {
  struct thread_args *args = arg;
  if (args->begin > args->end) {
    return NULL;
  }

  uint64_t partial = 1 % args->mod;
  for (int i = args->begin; i <= args->end; i++) {
    partial = (partial * (uint64_t)i) % args->mod;
  }

  pthread_mutex_lock(args->mutex);
  *(args->result) = (*(args->result) * partial) % args->mod;
  pthread_mutex_unlock(args->mutex);
  return NULL;
}

