#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "term.h"

static const double TOL = 1e-12;

void InsertTerm(term **poly, double coef, int exp)
{
    if (poly == NULL) return;

    if (exp < 0) {
        fprintf(stderr, "Error: exponent must be >= 0 (got %d).\n", exp);
        exit(EXIT_FAILURE);
    }
    if (fabs(coef) < TOL) return;

    term *prev = NULL;
    term *cur  = *poly;

    /* Keep list sorted by decreasing exponent */
    while (cur != NULL && cur->exp > exp) {
        prev = cur;
        cur  = cur->next;
    }

    /* Combine if same exponent exists */
    if (cur != NULL && cur->exp == exp) {
        cur->coef += coef;
        if (fabs(cur->coef) < TOL) {
            if (prev == NULL) *poly = cur->next;
            else prev->next = cur->next;
            free(cur);
        }
        return;
    }

    /* Insert new node between prev and cur */
    term *t = (term*)malloc(sizeof(term));
    if (!t) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    t->coef = coef;
    t->exp  = exp;
    t->next = cur;

    if (prev == NULL) *poly = t;
    else prev->next = t;
}