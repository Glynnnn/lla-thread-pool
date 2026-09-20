#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "threadpool.h"

void test_task(void *arg)
{
    printf("START thread %lu\n", (unsigned long)pthread_self());
    volatile unsigned long x = 0;

    for (unsigned long i = 0; i < 100000000; i++) {
        x += i;
    }
    printf("END thread %lu\n", (unsigned long)pthread_self());
}

int main() {
    threadpool_t pool;
    threadpool_init(&pool);

    // Add tasks to the thread pool
    for (int i = 0; i < 100; i++) {
        int* task_num = malloc(sizeof(int));
        if (task_num == NULL){
            printf("Malloc Failed\n");
            continue;
        }
        *task_num = i;
        threadpool_add_task(&pool, test_task, task_num);

    }

    // Let tasks complete
    sleep(5);

    threadpool_destroy(&pool);

    return 0;
}

