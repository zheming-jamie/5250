#include <stdio.h>
#include <math.h>
#include "term.h"

static const double TOL = 1e-12;

void PrintPoly(const term *poly, const char *name)
{
    if (name && name[0] != '\0') {
        printf("%s(x) = ", name);
    }

    if (poly == NULL) {
        printf("0\n");
        return;
    }

    int first = 1;
    for (const term *t = poly; t != NULL; t = t->next) {
        double c = t->coef;
        int e = t->exp;

        if (fabs(c) < TOL) continue;

        if (first) {
            if (c < 0) { printf("-"); c = -c; }
        } else {
            if (c < 0) { printf(" - "); c = -c; }
            else       { printf(" + "); }
        }

        if (e == 0) {
            printf("%.6g", c);
        } else {
            if (fabs(c - 1.0) > TOL) printf("%.6g*", c);
            printf("x");
            if (e != 1) printf("^%d", e);
        }

        first = 0;
    }

    if (first) printf("0");
    printf("\n");
}