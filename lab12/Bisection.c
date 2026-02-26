/* -------------------------------------------------------
 * Bisection.c
 * Implements the bisection root-finding method.
 *
 * Each iteration:
 *   1. Computes mid = (a + b) / 2
 *   2. Prints  a, b, mid  to screen
 *   3. Pushes the step onto the stack
 *   4. Narrows the interval based on sign of f(mid)
 *
 * After convergence the stack holds the full search history.
 * ------------------------------------------------------- */
#include <stdio.h>
#include <math.h>
#include "node.h"

void Bisection(double a, double b, double tol, int max_iter,
               node **top)
{
    /* --- validate bracket --- */
    double fa = UserFunction(a);
    double fb = UserFunction(b);

    if (fa * fb >= 0.0)
    {
        printf("\n Error: f(a) and f(b) must have opposite signs.\n");
        printf("        f(%.6f) = %.6f\n", a, fa);
        printf("        f(%.6f) = %.6f\n", b, fb);
        return;
    }

    printf("\n Bisection search for f(x) = 0  on [%.6f, %.6f]\n", a, b);
    printf(" Tolerance : %g    Max iterations: %d\n\n", tol, max_iter);
    printf(" %-6s  %-14s  %-14s  %-14s  %-14s\n",
           "Iter", "a", "b", "mid=(a+b)/2", "f(mid)");
    printf(" --------------------------------------------------------------------------\n");

    int    iter = 0;
    double mid  = a;

    while (iter < max_iter)
    {
        mid = (a + b) / 2.0;
        double fmid = UserFunction(mid);

        /* --- required output: a, b, (a+b)/2 --- */
        printf(" %-6i  %-14.8f  %-14.8f  %-14.8f  %-14.8f\n",
               iter + 1, a, b, mid, fmid);

        /* --- push this step onto the stack --- */
        Push(a, b, mid, top);

        /* --- check convergence --- */
        if (fabs(fmid) < tol || (b - a) / 2.0 < tol)
        {
            printf(" --------------------------------------------------------------------------\n");
            printf("\n Converged after %d iteration(s).\n", iter + 1);
            printf(" Approximate root: %.10f\n", mid);
            printf(" f(root)         : %.3e\n", fmid);
            return;
        }

        /* --- narrow the interval --- */
        if (fa * fmid < 0.0)
            b = mid;      /* root is in [a, mid] */
        else
        {
            a  = mid;     /* root is in [mid, b] */
            fa = fmid;
        }

        iter++;
    }

    printf(" --------------------------------------------------------------------------\n");
    printf("\n Warning: maximum iterations (%d) reached without convergence.\n", max_iter);
    printf(" Best estimate: %.10f\n", mid);
}
