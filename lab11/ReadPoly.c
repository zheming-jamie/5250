#include <stdio.h>
#include <stdlib.h>
#include "term.h"

void ReadPoly(term **poly, const char *name)
{
    if (poly == NULL) return;

    *poly = NULL;

    char prompt[128];
    snprintf(prompt, sizeof(prompt), "Enter number of terms for %s: ", name);
    int n = GetNumberOfTerms(prompt);

    if (n == 0) return;

    printf("Enter %d term(s) as: <coefficient> <exponent>\n", n);

    for (int i = 0; i < n; ++i) {
        double c;
        int e;
        printf("  %s term %d: ", name, i + 1);
        if (scanf("%lf %d", &c, &e) != 2) {
            fprintf(stderr, "\nError: failed to read coefficient/exponent.\n");
            exit(EXIT_FAILURE);
        }
        InsertTerm(poly, c, e);
    }
}