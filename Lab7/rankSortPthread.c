
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

#define N 10000
#define NUM_THREADS 4

static int *v, *res;

void initArrays()
{
    v = (int *)malloc(N * sizeof(int));
    res = (int *)malloc(N * sizeof(int));
    srand(time(NULL));

    for (int i = 0; i < N; ++i)
    {
        v[i] = rand();
    }
}

void compareVectors(int *a, int *b)
{
    for (int i = 0; i < N; i++)
    {
        if (a[i] != b[i])
        {
            printf("Vectors differ at index %d\n", i);
            return;
        }
    }
    printf("Vectors are sorted correctly\n");
}

int cmp(const void *a, const void *b)
{
    int A = *(int *)a;
    int B = *(int *)b;
    return A - B;
}

static void *rankSort(void *arg)
{
    int i, j, rank, x;
    long id = (long)arg;
    int start = id * N / NUM_THREADS;
    int end = (id + 1) * N / NUM_THREADS;

    for (i = start; i < end; i++)
    {
        rank = 0;
        x = v[i];
        for (j = 0; j < N; j++)
        {
            if (x > v[j])
                rank++;
        }
        res[rank] = x;
    }
    return NULL;
}

void printVector(int* v, int n)
{
   for (int i = 0; i < n; i++)
      printf("%d ", v[i]);
   printf("\n");
}

int main(int argc, char *argv[])
{
    long i;
    struct timeval start, end;

    initArrays();

    pthread_t *tids = (pthread_t *)malloc(NUM_THREADS * sizeof(pthread_t));

    gettimeofday(&start, NULL);

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&tids[i], NULL, rankSort, (void *)i);
    }

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(tids[i], NULL);
    }

    gettimeofday(&end, NULL);
    printf("Execution Time: %.4f s\n", end.tv_sec + end.tv_usec / 1000000.0 - start.tv_sec - start.tv_usec / 1000000.0);

    int *vQSort = malloc(sizeof(*vQSort) * N);
    for (i = 0; i != N; ++i)
    {
        vQSort[i] = v[i];
    }
    qsort(vQSort, N, sizeof(int), cmp);

    compareVectors(res, vQSort);

    free(v);
    free(res);
    free(tids);
    return 0;
}