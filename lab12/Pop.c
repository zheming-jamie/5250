/* -------------------------------------------------------
 * Pop.c
 * Removes the top node and returns its a, b, mid values.
 * Mirrors Pop.c from lecture 12.
 * ------------------------------------------------------- */
#include <stdlib.h>
#include "node.h"

void Pop(node **top, double *a, double *b, double *mid)
{
    if (*top == NULL)
        return;

    node *temp = (*top)->next;

    *a   = (*top)->a;
    *b   = (*top)->b;
    *mid = (*top)->mid;

    free(*top);
    *top = temp;

    /* decrement position of remaining nodes */
    node *ptr = *top;
    while (ptr != NULL)
    {
        ptr->position = ptr->position - 1;
        ptr = ptr->next;
    }
}
