#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

FILE *f1;
FILE *f2;
FILE *f3;
FILE *f4;
FILE *f5;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

int count = 0;

typedef struct
{
    int start;
    int finish;
} Args;

void primeornot(int prime)
{
    int i;
    int boolean = 0;

    if (prime < 2)
    {
        boolean = 1;
    }

    for (i = 2; i * i <= prime; i++)
    {
        if (prime % i == 0)
        {
            boolean = 1;
            break;
        }
    }

    if (boolean == 0)
    {
        pthread_mutex_lock(&count_mutex);
        fprintf(f5, " %d\n", prime);
        count++;
        pthread_mutex_unlock(&count_mutex);
    }
}

void *fun(void *vargs)
{
    int start, finish, i, num;
    Args *argu = (Args *)vargs;

    start = argu->start;
    finish = argu->finish;

    for (i = start; i <= finish; i++)
    {
        fscanf(f4, "%d", &num);
        primeornot(num);
    }
    pthread_exit(0);
}

int main()
{
    int num, i;
    int lines = 0;
    int threadcount;
    printf("Enter the number of threads: ");
    scanf("%d", &threadcount);

    int chunks[threadcount];
    Args *argu = (Args *)malloc(threadcount * sizeof(Args));

    pthread_t thread[threadcount];

    f1 = fopen("PrimeData1.txt", "r");
    f2 = fopen("PrimeData2.txt", "r");
    f3 = fopen("PrimeData3.txt", "r");
    f4 = fopen("DataCollections.txt", "w+");
    f5 = fopen("Primenumber.txt", "w");

    while (fscanf(f1, "%d", &num) != EOF)
    {
        lines++;
        fprintf(f4, "%d\n", num);
    }
    while (fscanf(f2, "%d", &num) != EOF)
    {
        lines++;
        fprintf(f4, "%d\n", num);
    }
    while (fscanf(f3, "%d", &num) != EOF)
    {
        lines++;
        fprintf(f4, "%d\n", num);
    }

    int remainder = lines % threadcount;

    fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f4);

    f1 = fopen("PrimeData1.txt", "r");
    f2 = fopen("PrimeData2.txt", "r");
    f3 = fopen("PrimeData3.txt", "r");

    for (i = 0; i < threadcount; i++)
    {
        chunks[i] = lines / threadcount;
    }

    for (i = 0; i < remainder; i++)
    {
        chunks[i] += 1;
    }

    for (i = 0; i < threadcount; i++)
    {
        if (i == 0)
        {
            argu[i].start = 0;
        }
        else
        {
            argu[i].start = argu[i - 1].finish + 1;
        }
        argu[i].finish = argu[i].start + chunks[i] - 1;
    }

    f4 = fopen("DataCollections.txt", "r");

    for (i = 0; i < threadcount; i++)
    {
        pthread_create(&thread[i], NULL, fun, &argu[i]);
    }

    for (i = 0; i < threadcount; i++)
    {
        pthread_join(thread[i], NULL);
    }

    fprintf(f5, "The sum of the number of prime numbers: %d\n", count);
    printf("The total number of prime numbers: %d\n", count);

    fclose(f4);
    fclose(f5);

    free(argu);
    pthread_mutex_destroy(&count_mutex);

    return 0;
}
