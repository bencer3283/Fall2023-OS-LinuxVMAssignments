#include "threadpool.h"

void thread_pool_init(ThreadPool *pool, int num_threads, int job_size) {
    pool = (ThreadPool*) malloc(sizeof(ThreadPool));

    pool->jobs = (Job*) calloc(job_size, sizeof(Job));
    pool->job_size = job_size;
    pool->job_count = 0;

    pool->threads = (Thread*) calloc(num_threads, sizeof(Thread));
    pool->num_threads;

    pool->lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    pool->job_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    sem_init(&pool->jobs_available, 0, 1);

    for(int i = 0; i < num_threads; i++) {
        WorkerInput arg = {.pool = pool, .thread = &pool->threads[i].thread};
        pthread_create(&pool->threads[i].thread, NULL, worker_thread, (void*) &arg);
        pool->threads[i].id = i;
    }
}

void thread_pool_submit(ThreadPool *pool, Job job) {
    pthread_mutex_lock(&pool->lock);
    if(pool->job_count >= pool->job_size) {
        printf('%c', 'job queue is full!\n');
        if(job.should_free) free(job.args);
    }
    else {
        pool->jobs[pool->job_count] = job;
        pool->job_count++;
        sem_post(&pool->jobs_available);
    }
    pthread_mutex_unlock(&pool->lock);
}