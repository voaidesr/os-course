#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define M 3
#define N 4
#define K 4
#define MAX_THREADS 17

int matA[M][N];
int matB[N][K];
int matC[M][K];
int step_i = 0;

typedef struct
{
    int start_row;
    int end_row;
} ThreadArgs;

void *multiply(void *arg)
{
    ThreadArgs *args = (ThreadArgs *) arg;
    for (int i = args->start_row; i < args->end_row; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                matC[i][j] += matA[i][k] * matB[k][j];

    free(arg);
    return NULL;
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matA[i][j] = rand() % 10;
            printf("%d ", matA[i][j]);
        }
        printf("\n");
    }

    printf("\n");


    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < K; j++)
        {
            matB[i][j] = rand() % 10;
            printf("%d ", matB[i][j]);
        }
        printf("\n");
    }

    pthread_t threads[MAX_THREADS];

    int thread_count = M < MAX_THREADS ? M : MAX_THREADS;
    int rows_per_thd = M / thread_count;
    int remainder = M % thread_count;
    int cur_row = 0;

    for (int i = 0; i < thread_count; i++)
    {
        ThreadArgs *args = malloc(sizeof(ThreadArgs));
        args->start_row = cur_row;
        int rows = rows_per_thd + (i < remainder ? 1 : 0);
        args->end_row = cur_row + rows;
        cur_row += rows;

        if (pthread_create(&threads[i], NULL, multiply, args) != 0)
        {
            perror("multiply");
            return 1;
        }
    }

    for (int i = 0; i < thread_count; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("\nResult Matrix C:\n");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            printf("%d ", matC[i][j]);
        }
        printf("\n");
    }

    return 0;
}
