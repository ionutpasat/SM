#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "sequential_multiplier.h"

int **a;         // A
int **b;         // B
int **c;         // Result
int matrix_size;

pthread_barrier_t barrier;
pthread_spinlock_t **spin;

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

static void priority_sum(int to_add, int i, int j) {
  pthread_spin_lock(&spin[i][j]);
  c[i][j] += to_add;
  pthread_spin_unlock(&spin[i][j]);
}

static void *threaded_multiply(void *args) {
  int id = *((int *)args);
  int i = (id / matrix_size) % matrix_size;
  int j = id % matrix_size;
  int k = id / (matrix_size * matrix_size);

  priority_sum(a[i][k] * b[k][j], i, j);

  return NULL;
}

int main() {

  clock_t start, end;

  srand(time(NULL));

  printf("Enter matrix size: ");
  scanf("%d", &matrix_size);

  a = (int **)calloc(matrix_size, sizeof(int *));
  b = (int **)calloc(matrix_size, sizeof(int *));
  c = (int **)calloc(matrix_size, sizeof(int *));
  spin =
      (pthread_spinlock_t **)calloc(matrix_size, sizeof(pthread_spinlock_t *));
  if (!a || !b || !c || !spin) {
    printf("Malloc failed\n");
    return 12;
  }

  for (int i = 0; i < matrix_size; ++i) {
    a[i] = (int *)calloc(matrix_size, sizeof(int));
    a[i] = (int *)calloc(matrix_size, sizeof(int));
    b[i] = (int *)calloc(matrix_size, sizeof(int));
    c[i] = (int *)calloc(matrix_size, sizeof(int));
    spin[i] =
        (pthread_spinlock_t *)calloc(matrix_size, sizeof(pthread_spinlock_t));
    if (!a[i] || !b[i] || !c[i] || !spin[i]) {
      printf("Malloc failed\n");
      return 12;
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

  // One lock for each zone of memory
  for (int i = 0; i < matrix_size; ++i) {
    for (int j = 0; j < matrix_size; ++j) {
      ret = pthread_spin_init(&spin[i][j], 0);
      if (ret != 0) {
        printf("pthread_spinlock_init failed\n");
        return 1;
      }
    }
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

  for (int i = 0; i < matrix_size; ++i) {
    for (int j = 0; j < matrix_size; ++j) {
      pthread_spin_destroy(&spin[i][j]);
    }
  }

  pthread_barrier_destroy(&barrier);

  // Print the result to console
  printf("Result:\n");
  print_matrix(c, matrix_size, matrix_size);
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
    free(a[i]);
    free(b[i]);
    free(c[i]);
    free((void *)spin[i]);
  }

  free(a);
  free(b);
  free(c);
  free(spin);

  return 0;
}