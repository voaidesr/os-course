#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_RESOURCES 15
int available_resources = MAX_RESOURCES;

pthread_mutex_t mtx;

int decrease_count ( int count )
{
    pthread_mutex_lock(&mtx);
    if ( available_resources < count )
    {
        pthread_mutex_unlock(&mtx);
        return -1;
    }
    else
        available_resources -= count ;
    pthread_mutex_unlock(&mtx);
    return 0;
}

int increase_count ( int count )
{
    pthread_mutex_lock(&mtx);
    available_resources += count ;
    pthread_mutex_unlock(&mtx);
    return 0;
}

void *foo(void *args)
{
    int *count = (int *)args;
    if (*count > available_resources)
        *count = 0;
    decrease_count(*count);
    printf("Got %d resources, %d remaining\n", *count, available_resources);
    increase_count(*count);
    printf("Freed %d resources, %d remaining\n", *count, available_resources);
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    printf("Total resources = %d\n", MAX_RESOURCES);

    if (pthread_mutex_init(&mtx, NULL) != 0)
    {
        perror("mutex");
        return 1;
    }

    int num_threads = rand() % 6 + 5;
    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);

    for (int i = 0; i < num_threads; i++)
    {
        int *arg = malloc(sizeof(int));
        *arg = rand() % MAX_RESOURCES + 1;
        pthread_create(&threads[i], NULL, foo, (void *)(arg));
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    free(threads);

    pthread_mutex_destroy(&mtx);
    return 0;
}
