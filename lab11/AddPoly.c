#include <stdlib.h>
#include "term.h"

term* AddPoly(const term *a, const term *b)
{
    term *sum = NULL;

    for (const term *t = a; t != NULL; t = t->next)
        InsertTerm(&sum, t->coef, t->exp);

    for (const term *t = b; t != NULL; t = t->next)
        InsertTerm(&sum, t->coef, t->exp);

    return sum;
}