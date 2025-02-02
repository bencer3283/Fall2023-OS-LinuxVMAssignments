/*
    Step 4: Synchronize: pthread_mutex_lock, pthread_mutex_unlock 
    http://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html

gcc parallel.cpp -o parallel -pthread
time ./parallel
diff parallel.txt serial.txt | wc -l
*/

#include <stdio.h>
#include <sys/time.h>
#include <stdint.h>
#include <pthread.h>

#define DIM 1000

#define NUM_THREADS 10 

long matrix_a[DIM][DIM];
long matrix_b[DIM][DIM];
long matrix_c[DIM][DIM];

struct thread_args{
    int start;
    int end;
};

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void init() {
    for(int i = 0; i < DIM; i++) {
        for(int j = 0; j < DIM; j++) {
            matrix_a[i][j] = i+j;
            matrix_b[i][j] = i-j;
            matrix_c[i][j] = 0;
        }
    }
}

void *multiply(void *arg) {
    struct thread_args * range = (struct thread_args *) arg;
    for(int i = 0; i < DIM; i++) {
        for(int j = 0; j < DIM; j++) {
            for(int k = range->start; k < range->end; k++) {
                pthread_mutex_lock(&lock);
                matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
                pthread_mutex_unlock(&lock);
            }
        }
    }
}

void print() {
    FILE *fp = fopen("parallel.txt","w");
    for(int i = 0; i < DIM; i++) {
        for(int j = 0; j < DIM; j++) {
            fprintf(fp, "%ld\n", matrix_c[i][j]);
        }
    }
    fclose(fp);
}

int main(void) {
    pthread_t child_threads[NUM_THREADS];
    struct thread_args work_ranges[NUM_THREADS];
    int current_start, range;
    current_start = 0;
    range = DIM / NUM_THREADS;
    for(int i = 0; i < NUM_THREADS; i++) {
        work_ranges[i].start = current_start;
        work_ranges[i].end = current_start + range;
        current_start += range;
    }
    work_ranges[NUM_THREADS-1].end = DIM;
    init();
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&child_threads[i], NULL, multiply, &work_ranges[i]);
    }
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(child_threads[i], NULL);
    }
    print();
    return 0;
}
