#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void *do_one_thing(void *arg);
static void *do_another_thing(void *arg);
static void do_wrap_up(int counter);

static int common = 0;

#ifdef USE_MUTEX
static pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
#define LOCK() pthread_mutex_lock(&counter_mutex)
#define UNLOCK() pthread_mutex_unlock(&counter_mutex)
#else
#define LOCK()
#define UNLOCK()
#endif

int main(void) {
  pthread_t thread1;
  pthread_t thread2;

  if (pthread_create(&thread1, NULL, do_one_thing, NULL) != 0) {
    perror("pthread_create");
    return EXIT_FAILURE;
  }

  if (pthread_create(&thread2, NULL, do_another_thing, NULL) != 0) {
    perror("pthread_create");
    return EXIT_FAILURE;
  }

  if (pthread_join(thread1, NULL) != 0) {
    perror("pthread_join");
    return EXIT_FAILURE;
  }

  if (pthread_join(thread2, NULL) != 0) {
    perror("pthread_join");
    return EXIT_FAILURE;
  }

  do_wrap_up(common);
  return EXIT_SUCCESS;
}

static void *do_one_thing(void *arg) {
  (void)arg;
  for (int i = 0; i < 50; i++) {
    LOCK();
    int work = common;
    printf("[thread A] counter = %d\n", work);
    work++;
    usleep(1000);
    common = work;
    UNLOCK();
  }
  return NULL;
}

static void *do_another_thing(void *arg) {
  (void)arg;
  for (int i = 0; i < 50; i++) {
    LOCK();
    int work = common;
    printf("[thread B] counter = %d\n", work);
    work++;
    usleep(1000);
    common = work;
    UNLOCK();
  }
  return NULL;
}

static void do_wrap_up(int counter) {
  printf("All done, counter = %d\n", counter);
}

