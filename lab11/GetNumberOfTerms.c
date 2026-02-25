#include <stdio.h>
#include <stdlib.h>
#include "term.h"

int GetNumberOfTerms(const char *prompt)
{
    int n;

    for (;;) {
        printf("%s", prompt);
        if (scanf("%d", &n) != 1) {
            fprintf(stderr, "\nError: failed to read an integer.\n");
            exit(EXIT_FAILURE);
        }
        if (n < 0) {
            printf("Please enter a non-negative integer.\n");
            continue;
        }
        return n;
    }
}