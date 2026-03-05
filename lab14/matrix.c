#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* ---------------- Matrix basics ---------------- */

Matrix* mat_create(int rows, int cols) {
    Matrix *A = (Matrix*)malloc(sizeof(Matrix));
    A->rows = rows;
    A->cols = cols;
    A->data = (double*)calloc((size_t)rows * (size_t)cols, sizeof(double));
    return A;
}

void mat_free(Matrix *A) {
    if (!A) return;
    free(A->data);
    free(A);
}

double mat_get(const Matrix *A, int i, int j) {
    return A->data[i * A->cols + j];
}

void mat_set(Matrix *A, int i, int j, double val) {
    A->data[i * A->cols + j] = val;
}

void mat_copy(Matrix *dst, const Matrix *src) {
    int n = src->rows * src->cols;
    dst->rows = src->rows;
    dst->cols = src->cols;
    for (int k = 0; k < n; ++k) dst->data[k] = src->data[k];
}

/* ---------------- Vector helpers ---------------- */

double vec_dot(const double *a, const double *b, int n) {
    double s = 0.0;
    for (int i = 0; i < n; ++i) s += a[i] * b[i];
    return s;
}

double vec_norm2(const double *x, int n) {
    return sqrt(vec_dot(x, x, n));
}

void vec_scale(double *x, int n, double s) {
    for (int i = 0; i < n; ++i) x[i] *= s;
}

void vec_copy(double *dst, const double *src, int n) {
    for (int i = 0; i < n; ++i) dst[i] = src[i];
}

/* ---------------- Linear algebra ---------------- */

void mat_vec_mul(const Matrix *A, const double *x, double *y) {
    for (int i = 0; i < A->rows; ++i) {
        double sum = 0.0;
        for (int j = 0; j < A->cols; ++j) {
            sum += mat_get(A, i, j) * x[j];
        }
        y[i] = sum;
    }
}

/* Gaussian elimination with partial pivoting.
   Works on a copied dense matrix (because we don't want to mutate A). */
int solve_gauss(const Matrix *A, const double *b, double *x) {
    const int n = A->rows;
    Matrix *M = mat_create(n, n);
    mat_copy(M, A);

    double *rhs = (double*)malloc((size_t)n * sizeof(double));
    for (int i = 0; i < n; ++i) rhs[i] = b[i];

    /* Forward elimination */
    for (int k = 0; k < n - 1; ++k) {
        /* Pivot selection */
        int pivot = k;
        double maxabs = fabs(mat_get(M, k, k));
        for (int i = k + 1; i < n; ++i) {
            double v = fabs(mat_get(M, i, k));
            if (v > maxabs) { maxabs = v; pivot = i; }
        }
        if (maxabs < 1e-14) { /* effectively singular */
            mat_free(M);
            free(rhs);
            return 1;
        }
        /* Swap rows if needed */
        if (pivot != k) {
            for (int j = k; j < n; ++j) {
                double tmp = mat_get(M, k, j);
                mat_set(M, k, j, mat_get(M, pivot, j));
                mat_set(M, pivot, j, tmp);
            }
            double tmpb = rhs[k];
            rhs[k] = rhs[pivot];
            rhs[pivot] = tmpb;
        }

        /* Eliminate below */
        double akk = mat_get(M, k, k);
        for (int i = k + 1; i < n; ++i) {
            double mik = mat_get(M, i, k) / akk;
            mat_set(M, i, k, 0.0);
            for (int j = k + 1; j < n; ++j) {
                mat_set(M, i, j, mat_get(M, i, j) - mik * mat_get(M, k, j));
            }
            rhs[i] -= mik * rhs[k];
        }
    }

    if (fabs(mat_get(M, n - 1, n - 1)) < 1e-14) {
        mat_free(M);
        free(rhs);
        return 1;
    }

    /* Back substitution */
    for (int i = n - 1; i >= 0; --i) {
        double sum = rhs[i];
        for (int j = i + 1; j < n; ++j) {
            sum -= mat_get(M, i, j) * x[j];
        }
        x[i] = sum / mat_get(M, i, i);
    }

    mat_free(M);
    free(rhs);
    return 0;
}

double rayleigh_quotient(const Matrix *A, const double *v, int n) {
    double *Av = (double*)malloc((size_t)n * sizeof(double));
    mat_vec_mul(A, v, Av);
    double num = vec_dot(v, Av, n);
    double den = vec_dot(v, v, n);
    free(Av);
    return num / den;
}

/* ---------------- Lab 14 eigenvalue solvers ---------------- */

int PowerIteration(const Matrix *A, double *v, int n,
                   double tol, int max_iters, double *lambda_out) {
    /* Normalize v */
    double nv = vec_norm2(v, n);
    if (nv == 0.0) return 0;
    vec_scale(v, n, 1.0 / nv);

    double lambda_prev = rayleigh_quotient(A, v, n);
    double *w = (double*)malloc((size_t)n * sizeof(double));

    int k = 0;
    while (1) {
        k++;
        mat_vec_mul(A, v, w);
        double nw = vec_norm2(w, n);
        if (nw == 0.0) break;
        vec_scale(w, n, 1.0 / nw);
        vec_copy(v, w, n);

        double lambda = rayleigh_quotient(A, v, n);
        if (fabs(lambda - lambda_prev) < tol || k >= max_iters) {
            *lambda_out = lambda;
            free(w);
            return k;
        }
        lambda_prev = lambda;
    }

    *lambda_out = lambda_prev;
    free(w);
    return k;
}

static void build_shifted_matrix(const Matrix *A, double mu, Matrix *B) {
    /* B = A - mu I */
    mat_copy(B, A);
    int n = A->rows;
    for (int i = 0; i < n; ++i) {
        mat_set(B, i, i, mat_get(B, i, i) - mu);
    }
}

int ShiftedInverseIteration(const Matrix *A, double mu, double *v, int n,
                            double tol, int max_iters, double *lambda_out) {
    /* Normalize v */
    double nv = vec_norm2(v, n);
    if (nv == 0.0) return 0;
    vec_scale(v, n, 1.0 / nv);

    double lambda_prev = rayleigh_quotient(A, v, n);

    double *w = (double*)malloc((size_t)n * sizeof(double));
    Matrix *B = mat_create(n, n);

    int k = 0;
    while (1) {
        k++;

        build_shifted_matrix(A, mu, B);

        /* Solve (A - mu I) w = v */
        int rc = solve_gauss(B, v, w);
        if (rc != 0) {
            /* Singular or near-singular; stop */
            break;
        }

        double nw = vec_norm2(w, n);
        if (nw == 0.0) break;
        vec_scale(w, n, 1.0 / nw);
        vec_copy(v, w, n);

        double lambda = rayleigh_quotient(A, v, n);
        if (fabs(lambda - lambda_prev) < tol || k >= max_iters) {
            *lambda_out = lambda;
            mat_free(B);
            free(w);
            return k;
        }
        lambda_prev = lambda;
    }

    *lambda_out = lambda_prev;
    mat_free(B);
    free(w);
    return k;
}

int RayleighQuotientIteration(const Matrix *A, double *v, int n,
                              double tol, int max_iters, double *lambda_out) {
    /* Normalize v */
    double nv = vec_norm2(v, n);
    if (nv == 0.0) return 0;
    vec_scale(v, n, 1.0 / nv);

    double lambda_prev = rayleigh_quotient(A, v, n);

    double *w = (double*)malloc((size_t)n * sizeof(double));
    Matrix *B = mat_create(n, n);

    int k = 0;
    while (1) {
        k++;

        /* Solve (A - lambda_prev I) w = v */
        build_shifted_matrix(A, lambda_prev, B);
        int rc = solve_gauss(B, v, w);
        if (rc != 0) {
            break;
        }

        double nw = vec_norm2(w, n);
        if (nw == 0.0) break;
        vec_scale(w, n, 1.0 / nw);
        vec_copy(v, w, n);

        double lambda = rayleigh_quotient(A, v, n);
        if (fabs(lambda - lambda_prev) < tol || k >= max_iters) {
            *lambda_out = lambda;
            mat_free(B);
            free(w);
            return k;
        }
        lambda_prev = lambda;
    }

    *lambda_out = lambda_prev;
    mat_free(B);
    free(w);
    return k;
}
