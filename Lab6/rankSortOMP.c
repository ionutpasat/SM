#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#define N 10000
#define NUM_THREADS 4

void initArrays(int** v, int** w) {
    *v = malloc(sizeof(**v) * N);
    *w = malloc(sizeof(**w) * N);
    srand(time(NULL));

    for (int i = 0; i < N; ++i) {
        (*v)[i] = rand();
    }
}

void compareVectors(int* a, int* b)
{
   int i;

   for (i = 0; i < N; i++)
   {
      if (a[i] != b[i])
      {
         printf("Vectors differ at index %d\n", i);
         return;
      }
   }
   printf("Vectors are sorted correctly\n");
}

int cmp(const void* a, const void* b)
{
    int A = *(int*)a;
    int B = *(int*)b;
   return A - B;
}

void printVector(int* v, int n)
{
   int i;

   for (i = 0; i < n; i++)
      printf("%d ", v[i]);
   printf("\n");
}

int main(int argc, char *argv[])
{
   double t1,t2;
   int *v, *w;
   int i, j, pos, tid;

   srand(time(NULL));
   initArrays(&v, &w);

   omp_set_num_threads(NUM_THREADS);

   #pragma omp parallel private(j,i,tid, pos)
   {
      tid = omp_get_thread_num();
      int rank;
      int start = tid * N / NUM_THREADS;
      int end = tid * N / NUM_THREADS + N / NUM_THREADS;
      t1 = omp_get_wtime();
      for (j = start; j < end; j++)
      {
         rank = v[j];
         pos = 0;
         #pragma omp parallel for reduction (+:pos)
         for (i = 0; i < N; i++)
            if ( rank > v[i] )
               pos++;
         w[pos] = rank;
      }
      t2 = omp_get_wtime();
   }

   int* vQSort = malloc(sizeof(*vQSort) * N);

   for (i = 0; i != N; ++i)
   {
      vQSort[i] = v[i];
   }
   qsort(vQSort, N, sizeof(int), cmp);


   compareVectors(w, vQSort);
   printf("Duration: %f\n",t2-t1);

   free(v);
   free(w);
   free(vQSort);

   return 0;
}