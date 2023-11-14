#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MIN(x, y) (x < y) ? x : y

#define N 10000
#define MASTER 0

void initArrays(int** v, int** pos, int rank, int start, int end) {
    *v = malloc(sizeof(**v) * N);
    srand(time(NULL));

    if (rank == MASTER) {

        for (int i = 0; i < N; ++i) {
            (*v)[i] = rand() % N;
        }

        *pos = calloc(N, sizeof(**pos));
    } else {
        *pos = calloc(end - start, sizeof(**pos));
    }
}

void compareVectors(int* a, int* b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            printf("Vectors differ at index %d!\n", i);
            return;
        }
    }
    printf("Vectors are sorted correctly\n");
}

void printVector(int* v, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

int cmp(const void* a, const void* b) {
    int A = *(int*)a;
    int B = *(int*)b;
    return A - B;
}

int main(int argc, char* argv[]) {
    int rank;
    int nProcesses;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    int start = rank * ceil((double)N / nProcesses);
    int end = MIN(N, (rank + 1) * ceil((double)N / nProcesses));

    int* pos;
    int* v;

    initArrays(&v, &pos, rank, start, end);

    MPI_Bcast(v, N, MPI_INT, MASTER, MPI_COMM_WORLD);

    MPI_Scatter(v, end - start, MPI_INT, v + start, end - start, MPI_INT, MASTER, MPI_COMM_WORLD);

    for (int i = start; i < end; ++i) {
        for (int j = 0; j < N; ++j) {
            if (v[j] < v[i] || (j < i && v[j] == v[i])) {
                pos[i - start]++;
            }
        }
    }

    MPI_Gather(
        pos,
        end - start,
        MPI_INT,
        pos,
        end - start,
        MPI_INT,
        MASTER,
        MPI_COMM_WORLD
    );

    if (rank == MASTER) {
        int* vQSort = malloc(sizeof(*vQSort) * N);
        int* w      = malloc(sizeof(*w) * N);

        for (int i = 0; i < N; ++i) {
            vQSort[i] = v[i];
        }
        qsort(vQSort, N, sizeof(int), cmp);

        for (int i = 0; i < N; ++i) {
            w[pos[i]] = v[i];
        }

        compareVectors(w, vQSort, N);

        free(vQSort);
        free(w);
    }

    free(v);
    free(pos);

    MPI_Finalize();

    return 0;
}
