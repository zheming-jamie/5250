/*
  COMS/CPRE/MATH 5250 — Lecture 8 Lab Assignment
  File: lab8.c

  - factorial(n): recursive, returns unsigned long long (no overflow check)
  - my_exp(x): Taylor expansion with x0 = round(x), e = 2.718281828459
  - Generates x = 0, 0.02, ..., 1 and writes (x, my_exp(x)) to a data file
*/

#include <stdio.h>
#include <math.h>

/* recursive factorial: unsigned long long, NO overflow check */
unsigned long long factorial(int n)
{
    if (n <= 1) return 1ULL;
    return (unsigned long long)n * factorial(n - 1);
}

/* exp(x) via: exp(x) = e^(x0) * exp(r),  x0=round(x), r=x-x0
   exp(r) approximated by Taylor sum_{k=0..N} r^k / k!
*/


double my_exp(double x)
{
    const double E = 2.718281828459;
    const int N = 20;

    /* nearest integer to x (half-up; ties go away from 0) */
    long long x0 = (x >= 0.0)
        ? (long long)floor(x + 0.5)
        : (long long)ceil (x - 0.5);

    double r = x - (double)x0;   /* now r is in about [-0.5, 0.5] */

    double s = 0.0;
    for (int k = 0; k <= N; k++) {
        s += pow(r, (double)k) / (double)factorial(k);
    }

    return pow(E, (double)x0) * s;
}
/*   only simple rounds
double my_exp(double x)
{
    const double E = 2.718281828459;   
    const int N = 20;                  
    long long x0 = llround(x);         
    double r = x - (double)x0;
    double s = 0.0;
    for (int k = 0; k <= N; k++) {
        s += pow(r, (double)k) / (double)factorial(k);
    }
    return pow(E, (double)x0) * s;
}
*/


int main(void)
{
    /* write output data */
    const char *outname = "lab8/exp.data";
    /*  5250 as running ls  */
    /*const char *outname = "exp.data";  if runs at lab8 as ls*/
    FILE *out = fopen(outname, "w");
    if (!out) {
        fprintf(stderr, "Error: cannot open output file '%s'\n", outname);
        return 1;
    }

    /* x = 0, 0.02, ..., 1  (51 points) */
    for (int i = 0; i <= 50; i++) {
        double x = 0.02 * (double)i;
        double y = my_exp(x);
        fprintf(out, "%.8f %.16e\n", x, y);
    }

    fclose(out);

    printf("Wrote 51 points to %s (columns: x  my_exp(x))\n", outname);
    return 0;
}
/*
compile and run:
ls as 5250
gcc lab8/lab8.c -o lab8/lab8 -lm

./lab8/lab8.exe

ls as lab8

*/