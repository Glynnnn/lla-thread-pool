#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "threadpool.h"


void threadpool_init(threadpool_t *pool){
    pthread_mutex_init(&(pool->lock), NULL);
    pthread_cond_init(&(pool->notify), NULL);

    pool->queued = 0;
    pool->queue_front = 0;
    pool->queue_back = 0;
    pool->stop = 0;

    for (int i = 0; i < THREADS; i++){
        pthread_create(&(pool->threads[i]), NULL, example_task, pool);
    }
    return;
}