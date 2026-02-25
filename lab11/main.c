#include <stdio.h>
#include <stdlib.h>
#include "term.h"

int main(void)
{
    term *P = NULL;
    term *Q = NULL;
    term *S = NULL;

    printf("Linked-list Polynomial Demo (addition + evaluation)\n\n");

    ReadPoly(&P, "P");
    ReadPoly(&Q, "Q");

    printf("\n");
    PrintPoly(P, "P");
    PrintPoly(Q, "Q");

    S = AddPoly(P, Q);
    PrintPoly(S, "S = P + Q");

    double x;
    printf("\nEnter x for evaluation: ");
    if (scanf("%lf", &x) != 1) {
        fprintf(stderr, "\nError: failed to read x.\n");
        DeletePoly(&P);
        DeletePoly(&Q);
        DeletePoly(&S);
        return EXIT_FAILURE;
    }

    printf("\nP(%.6g) = %.12g\n", x, EvalPoly(P, x));
    printf("Q(%.6g) = %.12g\n", x, EvalPoly(Q, x));
    printf("S(%.6g) = %.12g\n", x, EvalPoly(S, x));

    DeletePoly(&P);
    DeletePoly(&Q);
    DeletePoly(&S);

    return 0;
}