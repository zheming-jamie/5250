/* -------------------------------------------------------
 * Peek.c
 * Returns the midpoint of the top stack node (no removal).
 * ------------------------------------------------------- */
#include "node.h"

double Peek(node *top)
{
    return top->mid;
}
