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

// Cache-oblivious transpose function
void cache_oblivious(int M, int N, int A[N][M], int B[M][N], int r1, int c1, int r2, int c2, int rows, int cols) {
    // Base case: if the submatrix is small enough, perform a simple transpose
    if (rows <= 2 && cols <= 4) {
        int i, j;
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                B[c2 + j][r2 + i] = A[r1 + i][c1 + j];
            }
        }
        return;
    }

    // Recursively divide and conquer based on the larger dimension
    if (rows >= cols) {
        // Split the rows
        int mid = rows / 2;
        cache_oblivious(M, N, A, B, r1, c1, r2, c2, mid, cols);
        cache_oblivious(M, N, A, B, r1 + mid, c1, r2 + mid, c2, rows - mid, cols);
    } else {
        // Split the columns
        int mid = cols / 2;
        cache_oblivious(M, N, A, B, r1, c1, r2, c2, rows, mid);
        cache_oblivious(M, N, A, B, r1, c1 + mid, r2, c2 + mid, rows, cols - mid);
    }
}

/*
 * transpose_submit - Optimized transpose function using a cache-oblivious algorithm
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    // Call the cache-oblivious transpose function
    cache_oblivious(M, N, A, B, 0, 0, 0, 0, N, M);
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
    // Register the solution function
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
