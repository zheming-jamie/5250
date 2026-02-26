#ifndef __NODE_H__
#define __NODE_H__

/* -------------------------------------------------------
 * node.h
 * Each node stores one bisection step: a, b, midpoint.
 * The stack is LIFO (last-in = most-recent step on top).
 * ------------------------------------------------------- */

typedef struct node node;
struct node
{
    int    position; /* 1 = top of stack, increases downward */
    double a;        /* left  endpoint of current interval   */
    double b;        /* right endpoint of current interval   */
    double mid;      /* midpoint  (a+b)/2                    */
    node  *next;
};

/* Stack operations (lecture 12 style) */
void Push        (double a, double b, double mid, node **top);
void Pop         (node **top, double *a, double *b, double *mid);
double Peek      (node *top);           /* returns mid of top node */
void DisplayStack(node *top);
void PrintNode   (node *top);
void GetStackSize(node *top, int *stack_size);
void DeleteStack (node **top);

/* Bisection */
double UserFunction(double x);          /* f(x) defined in Function.c  */
void   Bisection   (double a, double b,
                    double tol, int max_iter,
                    node **top);

/* I/O helpers */
void   GetInterval (double *a, double *b);
double GetTolerance(void);
int    GetMaxIter  (void);

#endif
