#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "../inc/threadpool.h"

void* thread_func(void *arg){

    if (arg == NULL) return NULL;

    threadpool_t *pool = (threadpool_t*)arg;

    // func never stops until told to
    while(1){

        // lock the mutex
        pthread_mutex_lock(&(pool->lock));

        while(pool->queued == 0 && pool->queue_stop != 1){
            // if there are no tasks unlock the mutex and wait until condition recieves a signal
            printf("Thread waiting\n");
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }

        // if stop has been set and all queued tasks have been waited for to finish
        // exit the thread
        if (pool->queue_stop == 1 && pool->queued == 0){
            pthread_mutex_unlock(&(pool->lock));
            pthread_exit(NULL);
        }

        // call the next task in the queue and update the queue values
        task_t task = pool->queue[pool->queue_front];
        pool->queue_front = (pool->queue_front + 1) % QUEUE_SIZE;
        pool->queued--;

        // unlock mutex then call func so other threads not waiting for func to finich before mutex is unlocked
        pthread_mutex_unlock(&(pool->lock));

        task.fn(task.arg);

    }
    return NULL;

}

void threadpool_init(threadpool_t *pool){

    if (pool == NULL) return;

    // init the mutex and cond
    pthread_mutex_init(&(pool->lock), NULL);
    pthread_cond_init(&(pool->notify), NULL);
    
    // init the queue vals
    pool->queued = 0;
    pool->queue_front = 0;
    pool->queue_back = 0;
    pool->queue_stop = 0;
    
    // create the threads
    for (int i = 0; i < THREADS; i++){
        pthread_create(&(pool->threads[i]), NULL, thread_func, pool);
    }

}

void threadpool_add_function(threadpool_t *pool, void (*function)(void*), void *arg){

    if (pool == NULL || function == NULL) return;

    pthread_mutex_lock(&(pool->lock));

    // check the queue is not full

    if (pool->queued < QUEUE_SIZE){

        // add the function to the end of the queue
        pool->queue[pool->queue_back].fn = function;
        pool->queue[pool->queue_back].arg = arg;
        pool->queue_back = (pool->queue_back + 1) % QUEUE_SIZE;
        pool->queued++;

    }
    else{
        printf("Unable to add function: Queue is Full\n");
        free(arg);
    }

    pthread_cond_signal(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));

}

void threadpool_destroy(threadpool_t *pool){

    if (pool == NULL) return;

    // lock mutex and update stop value
    pthread_mutex_lock(&(pool->lock));
    pool->queue_stop = 1;
    
    // unlock mutex
    pthread_mutex_unlock(&(pool->lock));
    
    // broadcast notify to cond
    pthread_cond_broadcast(&(pool->notify));

    // join threads
    for (int i = 0; i < THREADS; i++){
        pthread_join(pool->threads[i], NULL);
    }

    // destroy mutex and cond
    pthread_mutex_destroy(&(pool->lock));
    pthread_cond_destroy(&(pool->notify));
}