#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void* reverse_string(void *arg)
{
    char *input = (char *) arg;
    int len = strlen(input);

    char *reversed = malloc((len + 1) * sizeof(char));

    if (reversed == NULL)
    {
        perror("Eroare la alocare\n");
        pthread_exit(NULL);
    }

    for (int i = 0; i < len; i++)
    {
        reversed[i] = input[len - i - 1];
    }
    reversed[len] = '\0';

    return (void *)reversed;
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("Utilizare %s <sir de caracter>\n", argv[0]);
        return 1;
    }

    pthread_t thread_id;
    char *input_str = argv[1];
    void *result;

    if (pthread_create(&thread_id, NULL, reverse_string, (void *)input_str) != 0)
    {
        perror("Eroare la creare thread.\n");
        return 1;
    }

    if (pthread_join(thread_id, &result) != 0)
    {
        perror("Eroare la join.\n");
        return 1;
    }

    if (result != NULL) {
        char *reversed_str = (char *)result;
        printf("%s\n", reversed_str);

        free(reversed_str);
    } else {
        printf("A apărut o eroare în procesarea șirului.\n");
    }

    return 0;
}
