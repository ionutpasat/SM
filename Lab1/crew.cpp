#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "sequential_multiplier.h"

int **a;         // A
int **b;         // B
int **c;         // Result
int ***v;        // 3D
int matrix_size;

pthread_barrier_t barrier;

//populate matrix with random numbers between 1 and 100
void populate_matrix_random(int **matrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      matrix[i][j] = rand() % 100 + 1;
    }
  }
}

void print_matrix(int **matrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
}

static int log_2(int nr) {
  for (int i = 0; i <= 31; ++i) {
    if (nr < (1 << i)) {
      return i - 1;
    }
  }
  return -1;
}

static void parallel_reduce(int i, int j, int k) {
  int log = log_2(matrix_size);
  for (int m = 0; m < log; ++m) {
    pthread_barrier_wait(&barrier);
    if (k % (1 << (m + 1)) == 0) {
      v[i][j][k] += v[i][j][k + (1 << m)];
    }
  }
}

static void *threaded_multiply(void *args) {
  int id = *((int *)args);
  int i = (id / matrix_size) % matrix_size;
  int j = id % matrix_size;
  int k = id / (matrix_size * matrix_size);
  int first;
  int second;

  pthread_barrier_wait(&barrier);
  first = a[i][k];

  pthread_barrier_wait(&barrier);
  second = b[k][j];

  v[i][j][k] = first * second;

  parallel_reduce(i, j, k);

  return NULL;
}

int main() {

  clock_t start, end;

  srand(time(NULL));

  printf("Enter matrix size: ");
  scanf("%d", &matrix_size);

  a = (int **)calloc(matrix_size, sizeof(int *));
  b = (int **)calloc(matrix_size, sizeof(int *));
  v = (int ***)calloc(matrix_size, sizeof(int **));
  if (!a || !b || !v) {
    printf("Malloc failed\n");
    return 12;
  }

  for (int i = 0; i < matrix_size; ++i) {
    a[i] = (int *)calloc(matrix_size, sizeof(int));
    b[i] = (int *)calloc(matrix_size, sizeof(int));
    v[i] = (int **)calloc(matrix_size, sizeof(int *));
    if (!a[i] || !b[i] || !v[i]) {
      printf("Malloc failed\n");
      return 12;
    }
  }

  for (int i = 0; i < matrix_size; ++i) {
    for (int j = 0; j < matrix_size; ++j) {
      v[i][j] = (int *)calloc(matrix_size, sizeof(int));
      if (!v[i][j]) {
        printf("Malloc failed\n");
        return 12;
      }
    }
  }

  populate_matrix_random(a, matrix_size, matrix_size);
  printf("Matrix A:\n");
  print_matrix(a, matrix_size, matrix_size);
  printf("\n");
  populate_matrix_random(b, matrix_size, matrix_size);
  printf("Matrix B:\n");
  print_matrix(b, matrix_size, matrix_size);
  printf("\n");

  const int nr_threads = matrix_size * matrix_size * matrix_size;
  int idx[nr_threads];
  pthread_t tid[nr_threads];
  int ret;

  ret = pthread_barrier_init(&barrier, NULL, nr_threads);
  if (ret != 0) {
    printf("pthread_barrier_init failed, %d\n", ret);
    return 1;
  }

  start = clock();

  for (int i = 0; i < nr_threads; ++i) {
    idx[i] = i;
    ret = pthread_create(&tid[i], NULL, &threaded_multiply, &idx[i]);
    if (ret != 0) {
      printf("pthread_create_init failed\n");
      return 1;
    }
  }

  for (int i = 0; i < nr_threads; ++i) {
    pthread_join(tid[i], NULL);
  }

  end = clock();

  pthread_barrier_destroy(&barrier);

  // Print result
  printf("Result:\n");
  for (int i = 0; i < matrix_size; ++i) {
    for (int j = 0; j < matrix_size; ++j) {
      printf("%d ", v[i][j][0]);
    }
    printf("\n");
  }
  printf("\n");

  printf("Time taken: %lf seconds\n\n", ((double)(end - start)) / CLOCKS_PER_SEC);
  
  // Compare with sequential result
  start = clock();
  sequential_multiplier(a, b, matrix_size, matrix_size, matrix_size);
  end = clock();
  printf("Sequential time taken: %lf seconds\n\n",
         ((double)(end - start)) / CLOCKS_PER_SEC);

  // Clean up memory
  for (int i = 0; i < matrix_size; ++i) {
    for (int j = 0; j < matrix_size; ++j) {
      free(v[i][j]);
    }
  }

  for (int i = 0; i < matrix_size; ++i) {
    free(a[i]);
    free(b[i]);
    free(v[i]);
  }

  free(a);
  free(b);
  free(v);
  return 0;
}