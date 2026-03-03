#include <stdlib.h>
#include <math.h>
#include "matrix.h"

matrix new_matrix(const int rows, const int cols)
{
    matrix A;
    A.rows = rows;
    A.cols = cols;
    A.val = (double*)malloc(sizeof(double) * rows * cols);
    for (int i = 0; i < rows * cols; ++i) A.val[i] = 0.0;
    return A;
}

vector new_vector(const int size)
{
    vector x;
    x.size = size;
    x.val = (double*)malloc(sizeof(double) * size);
    for (int i = 0; i < size; ++i) x.val[i] = 0.0;
    return x;
}

void free_matrix(matrix *A)
{
    free(A->val);
    A->val = 0;
    A->rows = 0;
    A->cols = 0;
}

void free_vector(vector *x)
{
    free(x->val);
    x->val = 0;
    x->size = 0;
}

static matrix matrix_copy(const matrix *A)
{
    matrix B = new_matrix(A->rows, A->cols);
    for (int i = 0; i < A->rows * A->cols; ++i) B.val[i] = A->val[i];
    return B;
}

static vector vector_copy(const vector *x)
{
    vector y = new_vector(x->size);
    for (int i = 0; i < x->size; ++i) y.val[i] = x->val[i];
    return y;
}

matrix matrix_transpose(const matrix *A)
{
    matrix AT = new_matrix(A->cols, A->rows);
    for (int i = 1; i <= A->rows; ++i) {
        for (int j = 1; j <= A->cols; ++j) {
            mget(AT, j, i) = mgetp(A, i, j);
        }
    }
    return AT;
}

matrix matrix_mult(const matrix *A, const matrix *B)
{
    matrix C = new_matrix(A->rows, B->cols);
    for (int i = 1; i <= A->rows; ++i) {
        for (int k = 1; k <= A->cols; ++k) {
            double aik = mgetp(A, i, k);
            for (int j = 1; j <= B->cols; ++j) {
                mget(C, i, j) += aik * mgetp(B, k, j);
            }
        }
    }
    return C;
}

vector matrix_vector_mult(const matrix *A, const vector *x)
{
    vector y = new_vector(A->rows);
    for (int i = 1; i <= A->rows; ++i) {
        double s = 0.0;
        for (int j = 1; j <= A->cols; ++j) {
            s += mgetp(A, i, j) * vgetp(x, j);
        }
        vget(y, i) = s;
    }
    return y;
}

vector solve(const matrix *A_in, const vector *b_in)
{
    // Gaussian elimination with partial pivoting + back substitution
    const int n = A_in->rows;

    matrix A = matrix_copy(A_in);
    vector b = vector_copy(b_in);

    for (int k = 1; k <= n - 1; ++k) {
        int pivot = k;
        double maxabs = fabs(mget(A, k, k));
        for (int i = k + 1; i <= n; ++i) {
            double a = fabs(mget(A, i, k));
            if (a > maxabs) { maxabs = a; pivot = i; }
        }

        if (pivot != k) {
            // swap rows pivot and k in A and b
            for (int j = k; j <= n; ++j) {
                double tmp = mget(A, k, j);
                mget(A, k, j) = mget(A, pivot, j);
                mget(A, pivot, j) = tmp;
            }
            double tb = vget(b, k);
            vget(b, k) = vget(b, pivot);
            vget(b, pivot) = tb;
        }

        double akk = mget(A, k, k);
        for (int i = k + 1; i <= n; ++i) {
            double mik = mget(A, i, k) / akk;
            mget(A, i, k) = 0.0;
            for (int j = k + 1; j <= n; ++j) {
                mget(A, i, j) -= mik * mget(A, k, j);
            }
            vget(b, i) -= mik * vget(b, k);
        }
    }

    vector x = new_vector(n);
    for (int i = n; i >= 1; --i) {
        double s = vget(b, i);
        for (int j = i + 1; j <= n; ++j) {
            s -= mget(A, i, j) * vget(x, j);
        }
        vget(x, i) = s / mget(A, i, i);
    }

    free_matrix(&A);
    free_vector(&b);
    return x;
}

double vector_norm2(const vector *x)
{
    double s = 0.0;
    for (int i = 1; i <= x->size; ++i) {
        double xi = vgetp(x, i);
        s += xi * xi;
    }
    return sqrt(s);
}

double relative_error(const vector *x_true, const vector *x_rec)
{
    double num = 0.0;
    double den = 0.0;
    for (int i = 1; i <= x_true->size; ++i) {
        double d = vgetp(x_true, i) - vgetp(x_rec, i);
        num += d * d;
        double xt = vgetp(x_true, i);
        den += xt * xt;
    }
    return sqrt(num) / sqrt(den);
}
