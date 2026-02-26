/* -------------------------------------------------------
 * DeleteStack.c
 * Frees every node in the stack.
 * Mirrors DeleteStack.c from lecture 12.
 * ------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include "node.h"

void DeleteStack(node **top)
{
    node *temp;
    while (*top != NULL)
    {
        temp = *top;
        *top = (*top)->next;
        free(temp);
    }

    printf("\n Stack freed. Goodbye!\n\n");
}
