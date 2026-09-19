#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "threadpool.h"


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
        // printf("Malloc %d\n", i);
        *task_num = i;
        threadpool_add_task(&pool, example_task, task_num);
        // free(task_num);
    }

    // Let tasks complete
    sleep(5);

    threadpool_destroy(&pool);

    return 0;
}

