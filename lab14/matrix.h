#ifndef MATRIX_H
#define MATRIX_H

/* Minimal matrix utilities for Lab 14 (eigenvalue solvers). */

typedef struct {
    int rows;
    int cols;
    double *data; /* row-major: data[i*cols + j] */
} Matrix;

/* ---- Matrix basics ---- */
Matrix* mat_create(int rows, int cols);
void    mat_free(Matrix *A);
double  mat_get(const Matrix *A, int i, int j);
void    mat_set(Matrix *A, int i, int j, double val);
void    mat_copy(Matrix *dst, const Matrix *src);

/* ---- Vector helpers (plain double arrays) ---- */
double vec_dot(const double *a, const double *b, int n);
double vec_norm2(const double *x, int n);          /* Euclidean norm */
void   vec_scale(double *x, int n, double s);
void   vec_copy(double *dst, const double *src, int n);

/* ---- Linear algebra ---- */
void   mat_vec_mul(const Matrix *A, const double *x, double *y);

/* Solve A x = b using Gaussian elimination with partial pivoting.
   Returns 0 on success, nonzero on failure. */
int    solve_gauss(const Matrix *A, const double *b, double *x);

/* Rayleigh quotient: (v^T A v) / (v^T v) */
double rayleigh_quotient(const Matrix *A, const double *v, int n);

/* ---- Lab 14 eigenvalue solvers ----
   Each routine overwrites v with the final normalized eigenvector estimate,
   writes eigenvalue estimate to *lambda_out, and returns iterations used.
*/
int PowerIteration(const Matrix *A, double *v, int n,
                   double tol, int max_iters, double *lambda_out);

int ShiftedInverseIteration(const Matrix *A, double mu, double *v, int n,
                            double tol, int max_iters, double *lambda_out);

int RayleighQuotientIteration(const Matrix *A, double *v, int n,
                              double tol, int max_iters, double *lambda_out);

#endif /* MATRIX_H */
