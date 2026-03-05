#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

/* Print to both stdout and an optional log file (same content). */
static void dual_printf(FILE *log, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);

    if (log) {
        va_start(ap, fmt);
        vfprintf(log, fmt, ap);
        va_end(ap);
        fflush(log);
    }
}

/* Build the same A matrix as Lab 13:
   A = (1/4) * tridiag(1, 2, 1) with zero boundary condition */
static Matrix* build_blur_matrix(int n) {
    Matrix *A = mat_create(n, n);
    for (int i = 0; i < n; ++i) {
        mat_set(A, i, i, 0.5); /* 2/4 */
        if (i - 1 >= 0) mat_set(A, i, i - 1, 0.25); /* 1/4 */
        if (i + 1 < n) mat_set(A, i, i + 1, 0.25);
    }
    return A;
}

/* True eigenvalues for A = (1/4) tridiag(1,2,1):
   λ_j = (1/4) * (2 + 2 cos(j*pi/(n+1))) = 0.5*(1 + cos(j*pi/(n+1))) */
static double true_lambda(int n, int j1_based) {
    double pi = acos(-1.0);
    double theta = (double)j1_based * pi / (double)(n + 1);
    return 0.5 * (1.0 + cos(theta));
}

static void run_case(int n, FILE *log) {
    Matrix *A = build_blur_matrix(n);

    const double tol = 1e-10;
    const int max_iters = 2000;

    double *v = (double*)malloc((size_t)n * sizeof(double));
    double lambda = 0.0;

    dual_printf(log, "\n============================\n");
    dual_printf(log, "Lab 14: eigenvalue solvers   \n");
    dual_printf(log, "Matrix A = (1/4) tridiag(1,2,1), n=%d\n", n);
    dual_printf(log, "tol=%.1e, max_iters=%d\n", tol, max_iters);

    /* ----- Power Iteration: dominant (largest) eigenvalue ----- */
    for (int i = 0; i < n; ++i) v[i] = 1.0; /* deterministic start */
    int it1 = PowerIteration(A, v, n, tol, max_iters, &lambda);

    double lambda_max_true = true_lambda(n, 1);     /* largest is j=1 */
    dual_printf(log, "\nPowerIteration():\n");
    dual_printf(log, "  lambda_hat = %.15f (iters=%d)\n", lambda, it1);
    dual_printf(log, "  true lambda_max = %.15f, abs error = %.3e\n",
                lambda_max_true, fabs(lambda - lambda_max_true));

    /* ----- Shifted Inverse Iteration: choose mu=2 (targets smallest) ----- */
    double mu1 = 2.0;
    for (int i = 0; i < n; ++i) v[i] = (double)(i + 1);
    int it2 = ShiftedInverseIteration(A, mu1, v, n, tol, max_iters, &lambda);

    double lambda_min_true = true_lambda(n, n);     /* smallest is j=n */
    dual_printf(log, "\nShiftedInverseIteration(mu=%.3f):\n", mu1);
    dual_printf(log, "  lambda_hat = %.15f (iters=%d)\n", lambda, it2);
    dual_printf(log, "  true lambda_min = %.15f, abs error = %.3e\n",
                lambda_min_true, fabs(lambda - lambda_min_true));

    /* ----- Shifted Inverse Iteration: choose mu=0 (targets eigenvalue closest to shift) ----- */
    double mu2 = 0.0;
    for (int i = 0; i < n; ++i) v[i] = (double)(i + 1);  /* deterministic start */
    int it3 = ShiftedInverseIteration(A, mu2, v, n, tol, max_iters, &lambda);

    /* find true eigenvalue closest to mu2 */
    double best = true_lambda(n, 1);
    for (int j = 2; j <= n; ++j) {
        double lj = true_lambda(n, j);
        if (fabs(lj - mu2) < fabs(best - mu2)) best = lj;
    }
    dual_printf(log, "\nShiftedInverseIteration(mu=%.3f):\n", mu2);
    dual_printf(log, "  lambda_hat = %.15f (iters=%d)\n", lambda, it3);
    dual_printf(log, "  closest true eigenvalue to mu = %.15f, abs error = %.3e\n",
                best, fabs(lambda - best));

    /* ----- Rayleigh Quotient Iteration ----- */
    for (int i = 0; i < n; ++i) v[i] = (double)(i + 1);  /* deterministic */
    int it4 = RayleighQuotientIteration(A, v, n, tol, max_iters, &lambda);

    /* find closest true eigenvalue (for reporting) */
    double closest = true_lambda(n, 1);
    for (int j = 2; j <= n; ++j) {
        double lj = true_lambda(n, j);
        if (fabs(lj - lambda) < fabs(closest - lambda)) closest = lj;
    }

    dual_printf(log, "\nRayleighQuotientIteration():\n");
    dual_printf(log, "  lambda_hat = %.15f (iters=%d)\n", lambda, it4);
    dual_printf(log, "  closest true eigenvalue to lambda_hat = %.15f, abs error = %.3e\n",
                closest, fabs(lambda - closest));

    free(v);
    mat_free(A);
}

int main(int argc, char **argv) {
    FILE *log = fopen("lab14_output.txt", "w");
    if (!log) {
        /* If file open fails, still run and print to console only. */
        log = NULL;
    }

    if (argc == 1) {
        /* default required sizes per lab slide */
        run_case(5, log);
        run_case(10, log);
        if (log) fclose(log);
        return 0;
    }

    int n = atoi(argv[1]);
    if (n <= 1) {
        dual_printf(log, "Usage: %s [n]\n", argv[0]);
        dual_printf(log, "Example: %s 10\n", argv[0]);
        if (log) fclose(log);
        return 0;
    }

    run_case(n, log);
    if (log) fclose(log);
    return 0;
}
