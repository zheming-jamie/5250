#include <stdlib.h>
#include "term.h"

void DeletePoly(term **poly)
{
    if (poly == NULL) return;

    term *cur = *poly;
    while (cur != NULL) {
        term *next = cur->next;
        free(cur);
        cur = next;
    }
    *poly = NULL;
}