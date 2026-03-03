#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef struct matrix matrix;
struct matrix {
    int rows;
    int cols;
    double *val;
};

typedef struct vector vector;
struct vector {
    int size;
    double *val;
};

// 1-based indexing macros (matches lecture style)
#define mget(mat,i,j)      ( (mat).val[((i)-1)*(mat).cols + ((j)-1)] )
#define mgetp(mat,i,j)     ( (mat)->val[((i)-1)*(mat)->cols + ((j)-1)] )
#define vget(vec,i)        ( (vec).val[(i)-1] )
#define vgetp(vec,i)       ( (vec)->val[(i)-1] )

matrix new_matrix(const int rows, const int cols);
vector new_vector(const int size);

void free_matrix(matrix *A);
void free_vector(vector *x);

matrix matrix_transpose(const matrix *A);
matrix matrix_mult(const matrix *A, const matrix *B);
vector matrix_vector_mult(const matrix *A, const vector *x);

vector solve(const matrix *A, const vector *b);

double vector_norm2(const vector *x);

double relative_error(const vector *x_true, const vector *x_rec);

#endif
