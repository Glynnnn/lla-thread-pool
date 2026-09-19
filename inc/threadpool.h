#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>


#define THREADS 8
#define QUEUE_SIZE 100

// the task type

typedef struct{

    void (*fn)(void *arg);
    void *arg;

} task_t;


typedef struct{

    // thread handlers
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t threads[THREADS];

    // task info
    task_t queue[QUEUE_SIZE];
    int queued;
    int queue_front;
    int queue_back;
    int stop;

} threadpool_t;


// Function declarations
void threadpool_init(threadpool_t *pool);
void threadpool_destroy(threadpool_t *pool);
void threadpool_add_task(threadpool_t *pool, void (*function)(void*), void* arg);
void example_task(void* arg);



#endif