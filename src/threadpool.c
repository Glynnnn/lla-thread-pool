#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "../inc/threadpool.h"

void* thread_func(void *arg){

    threadpool_t *pool = (threadpool_t*)arg;

    // thread runs constantly
    while (1){

        pthread_mutex_lock(&(pool->lock));
        
        // if no tasks then wait for notify
        while (pool->queued == 0 && pool->stop != 1){
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }
        // if stop is set leave the thread
        if (pool->stop == 1 && pool->queued == 0){
            pthread_mutex_unlock(&(pool->lock));
            pthread_exit(NULL);
        }
        
        // update values
        task_t task = pool->queue[pool->queue_front];
        pool->queued--;
        pool->queue_front = (pool->queue_front + 1) % QUEUE_SIZE;
        // unlock mutex
        pthread_mutex_unlock(&(pool->lock));

        // run the task 
        task.fn(task.arg);
    }

    return NULL;

}

void threadpool_init(threadpool_t *pool){
    pthread_mutex_init(&(pool->lock), NULL);
    pthread_cond_init(&(pool->notify), NULL);

    pool->queued = 0;
    pool->queue_front = 0;
    pool->queue_back = 0;
    pool->stop = 0;

    for (int i = 0; i < THREADS; i++){
        pthread_create(&(pool->threads[i]), NULL, thread_func, pool);
    }
    return;
}

void threadpool_destroy(threadpool_t *pool){
    pthread_mutex_lock(&(pool->lock));
    pool->stop = 1;
    pthread_cond_broadcast(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));

    for (int i = 0; i < THREADS; i++){
        pthread_join(pool->threads[i], NULL);
    }

    pthread_mutex_destroy(&(pool->lock));
    pthread_cond_destroy(&(pool->notify));

}



void threadpool_add_task(threadpool_t *pool, void (*function)(void*), void* arg){
    
    pthread_mutex_lock(&(pool->lock));

    if (pool->queued < QUEUE_SIZE){
        // add the fn to end of queue
        pool->queue[pool->queue_back].fn = function;
        pool->queue[pool->queue_back].arg = arg;
    
        // update queued
        pool->queue_back = (pool->queue_back + 1) % QUEUE_SIZE;
        pool->queued++;
    
        // signal notify
        pthread_cond_signal(&(pool->notify));

    }
    else{
        printf("Queue is full\n");
        free(arg);
    }
    // unlock thread
    pthread_mutex_unlock(&(pool->lock));
    return;
}
void example_task(void* arg) {
    int* num = (int*)arg;
    printf("Processing task %d\n", *num);
    sleep(1);  // Simulate task work
    free(arg);
}
