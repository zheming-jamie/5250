/*
  Lecture 7 Lab Assignment — lab.c
  Computes:
    (1) factorial(n) for integer n (stored in unsigned long long; NO overflow check)
    (2) exp(x)
    (3) log(y)
*/

#include <stdio.h>
#include <math.h>

int main(void)
{
    int n;
    double x, y;

    printf("Enter integer n for factorial (n >= 0): ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Input error: expected an integer for n.\n");
        return 1;
    }

    printf("Enter real x for exp(x): ");
    if (scanf("%lf", &x) != 1) {
        fprintf(stderr, "Input error: expected a real number for x.\n");
        return 1;
    }

    printf("Enter real y for log(y) (y > 0): ");
    if (scanf("%lf", &y) != 1) {
        fprintf(stderr, "Input error: expected a real number for y.\n");
        return 1;
    }

    printf("\n=== Results ===\n");

    /* factorial: NO overflow check (values wrap around for large n) */
    if (n < 0) {
        printf("factorial(%d): undefined for negative n\n", n);
    } else {
        unsigned long long f = 1ULL;
        for (int i = 2; i <= n; i++) {
            f *= (unsigned long long)i;
        }
        printf("factorial(%d) = %llu\n", n, f);
    }

    /* exp */
    printf("exp(%.10g) = %.16e\n", x, exp(x));

    /* log */
    if (y <= 0.0) {
        printf("log(%.10g): undefined for y <= 0\n", y);
    } else {
        printf("log(%.10g) = %.16e\n", y, log(y));
    }

    return 0;
}

/*
Compile & run (Unix):
  gcc lab8/lab.c -o lab8/lab -lm
  ./lab8/lab

If your system doesn't need -lm explicitly, gcc lab.c -o lab may work,
but -lm is the safe default for linking math functions.
*/
