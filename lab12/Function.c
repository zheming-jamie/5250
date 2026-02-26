/* -------------------------------------------------------
 * Function.c
 * Defines f(x) whose root the bisection method seeks.
 * Change only this file to solve a different equation.
 *
 * Current function: f(x) = x^3 - x - 2
 *   Real root near x ≈ 1.5214
 * ------------------------------------------------------- */
#include <math.h>
#include "node.h"

double UserFunction(double x)
{
    return x*x*x - x - 2.0;
}
