#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>

#define THREADS 8
#define QUEUE_SIZE 100

typedef struct{
    void (*fn)(void*);
    void *arg;
} task_t;

typedef struct{

    // thread fields
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t threads[THREADS];

    // queue fields
    task_t queue[QUEUE_SIZE];
    int queued;
    int queue_front;
    int queue_back;
    int queue_stop;

} threadpool_t;


void threadpool_init(threadpool_t *pool);
void threadpool_add_function(threadpool_t *pool, void (*function)(void*), void *arg);
void threadpool_destroy(threadpool_t *pool);


#endif