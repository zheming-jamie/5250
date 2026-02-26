/* -------------------------------------------------------
 * DisplayStack.c
 * Prints all bisection steps stored in the stack.
 * Step 1 (top) = most recent iteration.
 * Mirrors DisplayStack.c from lecture 12.
 * ------------------------------------------------------- */
#include <stdio.h>
#include "node.h"

void DisplayStack(node *top)
{
    if (top == NULL)
    {
        printf(" Stack is empty.\n");
        return;
    }

    printf("\n %-6s  %-14s  %-14s  %-14s\n",
           "Step", "a", "b", "mid=(a+b)/2");
    printf(" ----------------------------------------------------------\n");
    PrintNode(top);
    printf(" ----------------------------------------------------------\n");
}

void PrintNode(node *top)
{
    /* position 1 = top (most recent); print from bottom to top
       so the history reads chronologically.
       We use recursion to reverse the print order.            */
    if (top->next != NULL)
        PrintNode(top->next);

    printf(" %-6i  %-14.8f  %-14.8f  %-14.8f\n",
           top->position,   /* after reversal this is the iteration number */
           top->a, top->b, top->mid);
}
