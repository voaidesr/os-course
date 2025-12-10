#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int a;
pthread_mutex_t lock;

void *increment(void *args)
{
    for (int i = 0; i < 1000000; i++)
    {
        // pthread_mutex_lock(&lock);
        a++;
        // pthread_mutex_unlock(&lock);
    }
        return NULL;
}


int main(int argc, char const *argv[])
{
    pthread_t thread_id[2];

    for (int i = 0; i < 2; i++)
        if (pthread_create(&thread_id[i], NULL, increment, NULL) != 0)
        {
            perror("thread creation\n");
            return 1;
        }

    for (int i = 0; i < 2; i++)
        if (pthread_join(thread_id[i], NULL) != 0)
        {
            perror("thread sync");
            return 1;
        }

    printf("Variabila a = %d", a);

    return 0;
}
