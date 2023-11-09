#include "threadpool.h"
#include <stdlib.h>
#include <stdio.h>

void thread_pool_init(ThreadPool *pool, int num_threads, int job_size) {
    pool->jobs = (Job*) malloc(job_size * sizeof(Job));
    pool->job_size = job_size;
    pool->job_count = 0;
    pool->front = 0;

    pool->threads = (Thread*) malloc(num_threads * sizeof(Thread));
    pool->num_threads = num_threads;

    pool->lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    pool->job_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    sem_init(&pool->jobs_available, 0, 0); //?
    pool->stop_requested = 0;

    for(int i = 0; i < num_threads; i++) {
        WorkerInput* arg = (WorkerInput*) malloc(sizeof(WorkerInput));
        arg->pool = pool;
        arg->thread = &pool->threads[i];
        pthread_create(&pool->threads[i].thread, NULL, worker_thread, (void*) arg);
        pool->threads[i].id = i;
    }
}

void thread_pool_submit(ThreadPool *pool, Job job) {
    pthread_mutex_lock(&pool->lock);

    if(pool->job_count >= pool->job_size) {
        printf('job queue is full!\n');
        if(job.should_free) {
            free(job.args);
            job.is_freed = 1;
        } 
    }
    else {
        pool->jobs[pool->job_count] = job;
        pool->job_count++;
        sem_post(&pool->jobs_available);
    }
    pthread_mutex_unlock(&pool->lock);
    // printf("%d\n", pool->job_count);
}

void* worker_thread(void* args) {
    WorkerInput* input = (WorkerInput*) args;
    ThreadPool* pool = input->pool;
    Thread* thread = input->thread;

    int id = thread->id;
    int n_job = pool->num_threads;

    while (1)
    {
        sem_wait(&pool->jobs_available);
        if(pool->stop_requested) break;
        pthread_mutex_lock(&pool->lock);
        Job job = pool->jobs[pool->front];
        if(pool->front < pool->job_size - 1) pool->front++;
        pthread_mutex_unlock(&pool->lock);
        if(job.run_safely) {
            pthread_mutex_lock(&pool->job_lock);
            job.function(job.args);
            pthread_mutex_unlock(&pool->job_lock);
        }
        else job.function(job.args);
        if(job.should_free) {
            free(job.args);
            job.is_freed = 1;
            pool->jobs[job.id].is_freed = 1;
        }
    }
    printf("thread with id %d is finished.\n", thread->id);
    free(args);

}

void thread_pool_stop(ThreadPool *pool) {
    pool->stop_requested = 1;
    for(int i = 0; i < pool->num_threads; i++) {
        sem_post(&pool->jobs_available);
    }
}
void thread_pool_wait(ThreadPool *pool) {
    for(int i = 0; i < pool->num_threads; i++) {
        pthread_join(pool->threads[i].thread, NULL);
    }
}
void thread_pool_clean(ThreadPool *pool) {
    if(pool->job_count > 0) {
        for(int i = 0; i < pool->job_count; i++) {
            if(pool->jobs[i].should_free && !pool->jobs[i].is_freed) {
                free(pool->jobs[i].args);
            }
        }        
    }

    sem_destroy(&pool->jobs_available);
    pthread_mutex_destroy(&pool->lock);
    pthread_mutex_destroy(&pool->job_lock);

    free(pool->jobs);
    free(pool->threads);
    // free(pool);
}