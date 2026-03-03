#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"

// From the provided normal_distribution.c
double generate_normal(double sigma);

static matrix build_blur_matrix(int n)
{
    // A = (1/4) * tridiag(1,2,1) with zero boundary condition
    matrix A = new_matrix(n, n);
    for (int i = 1; i <= n; ++i) {
        mget(A, i, i) = 2.0 / 4.0;
        if (i > 1)   mget(A, i, i - 1) = 1.0 / 4.0;
        if (i < n)   mget(A, i, i + 1) = 1.0 / 4.0;
    }
    return A;
}

static vector build_test_signal(int n)
{
    // x_i = 1 for n/4 <= i <= n/2, else 0
    vector x = new_vector(n);
    int i1 = n / 4;
    int i2 = n / 2;
    for (int i = 1; i <= n; ++i) {
        vget(x, i) = (i >= i1 && i <= i2) ? 1.0 : 0.0;
    }
    return x;
}

static vector add_noise(const vector *b, double sigma)
{
    vector y = new_vector(b->size);
    for (int i = 1; i <= b->size; ++i) {
        vget(y, i) = vgetp(b, i) + generate_normal(sigma);
    }
    return y;
}

static void run_case(int n)
{
    double sigmas[3]  = {1e-6, 1e-4, 1e-2};
    double lambdas[3] = {1e-6, 1e-4, 1e-2};

    matrix A = build_blur_matrix(n);
    vector x_true = build_test_signal(n);
    vector b = matrix_vector_mult(&A, &x_true);

    // Precompute AT and ATA (A is symmetric, but we follow formula)
    matrix AT = matrix_transpose(&A);
    matrix ATA = matrix_mult(&AT, &A);

    printf("\n==== n = %d ====\n", n);

    for (int s = 0; s < 3; ++s) {
        double sigma = sigmas[s];
        vector b_noisy = add_noise(&b, sigma);

        // Unregularized solve: A x_rec = b_noisy
        vector x_rec = solve(&A, &b_noisy);
        double err_unreg = relative_error(&x_true, &x_rec);
        printf("sigma = %.0e | unreg relerr = %.6e\n", sigma, err_unreg);

        // Regularized: (A^T A + lambda I) x = A^T b_noisy
        vector ATb = matrix_vector_mult(&AT, &b_noisy);

        for (int l = 0; l < 3; ++l) {
            double lambda = lambdas[l];

            matrix M = new_matrix(n, n);
            for (int i = 1; i <= n; ++i) {
                for (int j = 1; j <= n; ++j) {
                    mget(M, i, j) = mget(ATA, i, j);
                }
                mget(M, i, i) += lambda;
            }

            vector x_reg = solve(&M, &ATb);
            double err_reg = relative_error(&x_true, &x_reg);
            printf("           lambda = %.0e | reg  relerr = %.6e\n", lambda, err_reg);

            free_matrix(&M);
            free_vector(&x_reg);
        }

        free_vector(&ATb);
        free_vector(&x_rec);
        free_vector(&b_noisy);
    }

    free_matrix(&ATA);
    free_matrix(&AT);
    free_vector(&b);
    free_vector(&x_true);
    free_matrix(&A);
}

int main(int argc, char **argv)
{
    srand((unsigned)time(NULL));

    if (argc == 2) {
        int n = atoi(argv[1]);
        run_case(n);
    } else {
        run_case(64);
        run_case(128);
    }

    return 0;
}
