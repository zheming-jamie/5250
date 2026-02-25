#include <math.h>
#include "term.h"

double EvalPoly(const term *poly, double x)
{
    double y = 0.0;
    for (const term *t = poly; t != NULL; t = t->next) {
        y += t->coef * pow(x, (double)t->exp);
    }
    return y;
}