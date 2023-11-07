#include "threadpool.h"
#include <stdlib.h>
#include <stdio.h>

void thread_pool_init(ThreadPool *pool, int num_threads, int job_size) {
    pool = (ThreadPool*) malloc(sizeof(ThreadPool));

    pool->jobs = (Job*) malloc(job_size * sizeof(Job));
    pool->job_size = job_size;
    pool->job_count = 0;

    pool->threads = (Thread*) malloc(num_threads * sizeof(Thread));
    pool->num_threads = num_threads;

    pool->lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    pool->job_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    sem_init(&pool->jobs_available, 0, num_threads);

    WorkerInput args[num_threads];
    for(int i = 0; i < num_threads; i++) {
        args[i] = (WorkerInput){.pool = pool, .thread = &pool->threads[i]};
    }
    for(int i = 0; i < num_threads; i++) {
        //pthread_create(&pool->threads[i].thread, NULL, worker_thread, (void*) &args[i]);
        pool->threads[i].id = i;
    }

    return 0;
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
}

void* worker_thread(void* args) {
    WorkerInput* input = (WorkerInput*) args;
    ThreadPool* pool = input->pool;
    Thread* thread = input->thread;

    while (1)
    {
        sem_wait(&pool->jobs_available);
        if(pool->stop_requested) break;
        // pthread_mutex_lock(&pool->lock);
        // Job job = pool->jobs[pool->front];
        // if(pool->front < pool->job_size - 2) pool->front =  pool->front + 1;
        // else thread_pool_stop(pool);
        // pthread_mutex_unlock(&pool->lock);
        // if(job.run_safely) {
        //     pthread_mutex_lock(&pool->job_lock);
        //     job.function(job.args);
        //     pthread_mutex_unlock(&pool->job_lock);
        // }
        // else job.function(job.args);
        // if(job.should_free) {
        //     free(job.args);
        //     job.is_freed = 1;
        // }
        // sem_post(&pool->jobs_available);
        // printf('thread with id %d is finished.\n', thread->id);
        // free(input);
    }


}

void thread_pool_stop(ThreadPool *pool) {
    pool->stop_requested = 1;
    sem_post(&pool->jobs_available);
}
void thread_pool_wait(ThreadPool *pool) {
    for(int i = 0; i < pool->num_threads; i++) {
        pthread_join(pool->threads[i].thread, NULL);
    }
}
void thread_pool_clean(ThreadPool *pool) {
    for(int i = 0; i < pool->job_count; i++) {
        if(pool->jobs[i].should_free == 1 && pool->jobs[i].is_freed == 0) {
            free(pool->jobs[i].args);
        }
    }

    sem_destroy(&pool->jobs_available);
    pthread_mutex_destroy(&pool->lock);
    pthread_mutex_destroy(&pool->job_lock);

    free(pool->jobs);
    free(pool->threads);
    free(pool);
}