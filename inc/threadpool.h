#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>


#define THREADS 6
#define QUEUE_SIZE 20

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
    int queue_size;
    int queue_front;
    int queue_back;
    int queue_stop;

} threadpool_t;


// Function declarations
void threadpool_init(threadpool_t *pool);
void threadpool_destroy(threadpool_t *pool);
void threadpool_add_task(threadpool_t *pool, void (*function)(void*), void* arg);
void example_task(void* arg);



#endif