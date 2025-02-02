/*
    Step 0: Let's start with the following sequential baseline

gcc serial.cpp -o serial
time ./serial
*/

#include <stdio.h>

#define DIM 1000

long matrix_a[DIM][DIM];
long matrix_b[DIM][DIM];
long matrix_c[DIM][DIM];

void init() {
    for(int i = 0; i < DIM; i++) {
        for(int j = 0; j < DIM; j++) {
            matrix_a[i][j] = i+j;
            matrix_b[i][j] = i-j;
            matrix_c[i][j] = 0;
        }
    }
}

void multiply() {
    for(int i = 0; i < DIM; i++) {
        for(int j = 0; j < DIM; j++) {
            for(int k = 0; k < DIM; k++) {
                matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
        }
    }
}

void print() {
    FILE *fp = fopen("serial.txt","w");
    for(int i = 0; i < DIM; i++) {
        for(int j = 0; j < DIM; j++) {
            fprintf(fp, "%ld\n", matrix_c[i][j]);
        }
    }
    fclose(fp);
}

int main(void) {
    init();
    multiply();
    print();
    return 0;
}
