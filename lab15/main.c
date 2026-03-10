#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "matrix.h"
#include "trimatrix.h"

void Hessenberg(const matrix* Ain, trimatrix* T);
void QRA(trimatrix* T);

static int cmp_desc(const void* a, const void* b)
{
    const double x = *(const double*)a;
    const double y = *(const double*)b;
    if (x < y) return 1;
    if (x > y) return -1;
    return 0;
}

static matrix build_lab13_matrix(const int N)
{
    assert(N > 1);
    matrix A = new_matrix(N, N);

    for (int i = 1; i <= N; ++i)
    {
        mget(A, i, i) = 0.5;
        if (i > 1) mget(A, i, i - 1) = 0.25;
        if (i < N) mget(A, i, i + 1) = 0.25;
    }

    return A;
}

static void print_sorted_eigenvalues(const trimatrix* T)
{
    const int N = T->rows;
    double* eig = (double*)malloc((size_t)N * sizeof(double));
    assert(eig != NULL);

    for (int i = 0; i < N; ++i)
    {
        eig[i] = tget(T, i + 1, i + 1);
    }

    qsort(eig, (size_t)N, sizeof(double), cmp_desc);

    printf("Sorted eigenvalues (descending):\n");
    for (int i = 0; i < N; ++i)
    {
        printf("lambda_%d = %.15e\n", i + 1, eig[i]);
    }
    printf("\n");

    free(eig);
}

static void run_case(const int N)
{
    matrix A = build_lab13_matrix(N);
    trimatrix T = new_trimatrix(N);

    printf("========================================\n");
    printf("Lab 15 QR algorithm on Lecture 13 matrix\n");
    printf("N = %d\n", N);
    printf("========================================\n");

    printf("Original matrix A:\n");
    print_matrix(&A);

    Hessenberg(&A, &T);
    printf("After Hessenberg reduction (tridiagonal T):\n");
    print_trimatrix(&T);

    QRA(&T);
    printf("After QR algorithm:\n");
    print_trimatrix(&T);

    print_sorted_eigenvalues(&T);

    delete_trimatrix(&T);
    delete_matrix(&A);
}

int main(void)
{
    run_case(6);
    run_case(12);
    return 0;
}