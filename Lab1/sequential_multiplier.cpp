#include <stdio.h>
#include "sequential_multiplier.h"

void sequential_multiplier(int **matrix1, int **matrix2, int m, int n, int p) {
    printf("Sequential multiplier result:\n");

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            int result = 0;
            for (int k = 0; k < n; k++) {
                result += matrix1[i][k] * matrix2[k][j];
            }
            printf("%d ", result);
        }
        printf("\n");
    }
    printf("\n");
}