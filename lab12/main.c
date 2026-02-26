/* -------------------------------------------------------
 * main.c
 * Lab assignment: Stack applied to the Bisection method.
 *
 * Workflow:
 *   1. Ask the user for [a, b], tolerance, max iterations.
 *   2. Run Bisection — each step is printed AND pushed
 *      onto the stack.
 *   3. Display the complete stack (search history).
 *   4. Report total steps via GetStackSize.
 *   5. Free the stack with DeleteStack.
 *
 * Compile:  make
 * Run:      ./main.exe
 * ------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include "node.h"

int main(void)
{
    /* --- initialise empty stack --- */
    node *top = NULL;

    /* --- get user inputs --- */
    double a, b;
    GetInterval(&a, &b);
    double tol      = GetTolerance();
    int    max_iter = GetMaxIter();

    /* --- run bisection; each step is pushed to *top --- */
    Bisection(a, b, tol, max_iter, &top);

    /* --- display the full search history from the stack --- */
    printf("\n ============================================================\n");
    printf("  Stack contents (complete bisection search history)\n");
    printf("  Step 1 = first iteration, last step = final iteration\n");
    printf(" ============================================================");
    DisplayStack(top);

    /* --- report number of steps stored --- */
    int size = 0;
    GetStackSize(top, &size);
    printf(" Total steps stored in stack: %d\n", size);

    /* --- free memory --- */
    DeleteStack(&top);

    return 0;
}
