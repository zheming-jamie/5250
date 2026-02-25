#ifndef __TERM_H__
#define __TERM_H__

#include <stddef.h> /* for NULL */

typedef struct term term;

struct term {
    int exp;        /* exponent (>=0) */
    double coef;    /* coefficient */
    term *next;
};

/* Input helpers */
int GetNumberOfTerms(const char *prompt);
void ReadPoly(term **poly, const char *name);

/* Linked-list ops for polynomials */
void InsertTerm(term **poly, double coef, int exp);
term* AddPoly(const term *a, const term *b);
double EvalPoly(const term *poly, double x);
void PrintPoly(const term *poly, const char *name);
void DeletePoly(term **poly);

#endif /* __TERM_H__ */