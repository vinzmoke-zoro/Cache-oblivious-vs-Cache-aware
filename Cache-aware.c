// Copyright 2024 - Imtiaz Rahman
/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include <stdlib.h>
#include "cachelab.h"

// Helper function to check if B is the transpose of A
int is_transpose(int M, int N, int A[N][M], int B[M][N]);

// Cache-aware transpose function for 32x32, 64x64, and 61x67 matrices
void cache_aware_transpose(int M, int N, int A[N][M], int B[M][N]) {
    int row, col, i, j;
    int temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8;
    // Optimize for 32x32 matrix using 8x8 blocks
    if (M == 32) {
        for (row = 0; row < N; row += 8) {
            for (col = 0; col < M; col += 8) {
                for (i = row; i < row + 8; ++i) {
                    temp1 = A[i][col];
                    temp2 = A[i][col + 1];
                    temp3 = A[i][col + 2];
                    temp4 = A[i][col + 3];
                    temp5 = A[i][col + 4];
                    temp6 = A[i][col + 5];
                    temp7 = A[i][col + 6];
                    temp8 = A[i][col + 7];

                    B[col][i] = temp1;
                    B[col + 1][i] = temp2;
                    B[col + 2][i] = temp3;
                    B[col + 3][i] = temp4;
                    B[col + 4][i] = temp5;
                    B[col + 5][i] = temp6;
                    B[col + 6][i] = temp7;
                    B[col + 7][i] = temp8;
                }
            }
        }
    }
    // Optimize for 64x64 matrix using a combination of 4x4 and 8x8 blocks
    else if (M == 64) {
        for (row = 0; row < N; row += 8) {
            for (col = 0; col < M; col += 8) {
                for (i = row; i < row + 4; ++i) {
                    temp1 = A[i][col];
                    temp2 = A[i][col + 1];
                    temp3 = A[i][col + 2];
                    temp4 = A[i][col + 3];
                    temp5 = A[i][col + 4];
                    temp6 = A[i][col + 5];
                    temp7 = A[i][col + 6];
                    temp8 = A[i][col + 7];

                    B[col][i] = temp1;
                    B[col][i + 4] = temp5;
                    B[col + 1][i] = temp2;
                    B[col + 1][i + 4] = temp6;
                    B[col + 2][i] = temp3;
                    B[col + 2][i + 4] = temp7;
                    B[col + 3][i] = temp4;
                    B[col + 3][i + 4] = temp8;
                }
                for (j = col; j < col + 4; ++j) {
                    temp1 = B[j][row + 4];
                    temp2 = B[j][row + 5];
                    temp3 = B[j][row + 6];
                    temp4 = B[j][row + 7];
                    temp5 = A[row + 4][j];
                    temp6 = A[row + 5][j];
                    temp7 = A[row + 6][j];
                    temp8 = A[row + 7][j];

                    B[j][row + 4] = temp5;
                    B[j][row + 5] = temp6;
                    B[j][row + 6] = temp7;
                    B[j][row + 7] = temp8;
                    B[j + 4][row] = temp1;
                    B[j + 4][row + 1] = temp2;
                    B[j + 4][row + 2] = temp3;
                    B[j + 4][row + 3] = temp4;
                }
                for (i = row + 4; i < row + 8; ++i) {
                    temp1 = A[i][col + 4];
                    temp2 = A[i][col + 5];
                    temp3 = A[i][col + 6];
                    temp4 = A[i][col + 7];

                    B[col + 4][i] = temp1;
                    B[col + 5][i] = temp2;
                    B[col + 6][i] = temp3;
                    B[col + 7][i] = temp4;
                }
            }
        }
    }
    // General case using block size of 16x16
    else {
        const int block_size = 16;
        for (row = 0; row < N; row += block_size) {
            for (col = 0; col < M; col += block_size) {
                for (i = row; i < row + block_size && i < N; ++i) {
                    for (j = col; j < col + block_size && j < M; ++j) {
                        B[j][i] = A[i][j];
                    }
                }
            }
        }
    }
}

/*
 * transpose_submit - Optimized transpose function using a cache-aware algorithm
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    // Use the cache-aware algorithm to perform the transpose
    cache_aware_transpose(M, N, A, B);
}

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    // Perform a simple row-wise transpose
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}

/*
 * registerFunctions - This function registers the transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    // Registers the solution function
    registerTransFunction(transpose_submit, transpose_submit_desc);

    // Registers any additional transpose functions
    registerTransFunction(trans, trans_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    // Check each element to see if B is the transpose of A
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0; // Not a transpose
            }
        }
    }
    return 1; // Is a transpose
}
