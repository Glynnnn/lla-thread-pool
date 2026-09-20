#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "threadpool.h"


void example_task(void *arg){

    int *num = (int*)arg;
    printf("Process is running on: %d\n", *num);
    sleep(1);
    free(arg);

}


int main() {
    threadpool_t pool;
    threadpool_init(&pool);

    // Add tasks to the thread pool
    for (int i = 0; i < 16; i++) {
        int* task_num = malloc(sizeof(int));
        if (task_num == NULL){
            printf("Malloc Failed\n");
            continue;
        }
        *task_num = i;
        threadpool_add_function(&pool, example_task, task_num);

    }

    // Let tasks complete
    sleep(5);

    threadpool_destroy(&pool);

    return 0;
}

