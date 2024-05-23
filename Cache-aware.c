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
    int block_size = 8; // Choosing a block size that fits well in the cache

    if (M == 32 && N == 32) {
        block_size = 8;
    } else if (M == 64 && N == 64) {
        block_size = 4;
    } else if (M == 61 && N == 67) {
        block_size = 8;
    }

    for (int i = 0; i < N; i += block_size) {
        for (int j = 0; j < M; j += block_size) {
            for (int k = i; k < i + block_size && k < N; k++) {
                for (int l = j; l < j + block_size && l < M; l++) {
                    B[l][k] = A[k][l];
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
    // Register solution function
    registerTransFunction(transpose_submit, transpose_submit_desc);

    // Register any additional transpose functions
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
