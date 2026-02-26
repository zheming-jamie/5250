/* -------------------------------------------------------
 * GetInput.c
 * Collects all user inputs needed for bisection.
 * ------------------------------------------------------- */
#include <stdio.h>
#include "node.h"

void GetInterval(double *a, double *b)
{
    printf("\n Enter left  endpoint a: ");
    scanf("%lf", a);
    printf(" Enter right endpoint b: ");
    scanf("%lf", b);
}

double GetTolerance(void)
{
    double tol;
    printf(" Enter tolerance (e.g. 1e-6): ");
    scanf("%lf", &tol);
    return tol;
}

int GetMaxIter(void)
{
    int n;
    printf(" Enter maximum iterations  : ");
    scanf("%i", &n);
    return n;
}
