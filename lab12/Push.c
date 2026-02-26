/* -------------------------------------------------------
 * Push.c
 * Adds a new bisection step to the top of the stack.
 * Mirrors the Push.c from lecture 12 exactly, extended
 * to store three doubles instead of one int.
 * ------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include "node.h"

void Push(double a, double b, double mid, node **top)
{
    node *temp = (node *)malloc(sizeof(node));
    if (temp == NULL)
    {
        printf(" Error: malloc failed in Push.\n");
        exit(1);
    }

    temp->a   = a;
    temp->b   = b;
    temp->mid = mid;

    if (*top == NULL)
    {
        temp->position = 1;
        temp->next     = NULL;
        *top           = temp;
    }
    else
    {
        temp->position = 1;
        temp->next     = *top;
        *top           = temp;

        /* increment position of every node below the new top */
        node *ptr = (*top)->next;
        while (ptr != NULL)
        {
            ptr->position = ptr->position + 1;
            ptr = ptr->next;
        }
    }
}
