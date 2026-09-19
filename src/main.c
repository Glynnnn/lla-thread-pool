#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "threadpool.h"

int main(){

    threadpool_t pool;
    threadpool_init(&pool);

    for (int i = 0; i < 100; i++){
        int *val = malloc(sizeof(int));
        *val = i;
        threadpool_add_task(&pool, example_task, val);
    }

    sleep(15);

    threadpool_destroy(&pool);

    return 0;
}

