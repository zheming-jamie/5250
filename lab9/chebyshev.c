/*
  chebyshev.c (Lab 9)

  Chebyshev basis (degree <= 5):
    phi_0(x)=1
    phi_1(x)=x
    phi_2(x)=2x^2-1
    phi_3(x)=4x^3-3x
    phi_4(x)=8x^4-8x^2+1
    phi_5(x)=16x^5-20x^3+5x

  p_N(x) = sum_{k=0..N} b[k] * phi_k(x),  0 <= N <= 5

  Output: lab9/poly.data
  Plot:   lab9/plot_chebyshev.py (called via .venv python)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static void SamplePoly(const int N,
                       const int NumPts,
                       const double b[],
                       const double x[],
                       double y[]);

static void WritePoly(const char* filepath,
                      const int NumPts,
                      const double x[],
                      const double y[]);

int main(void)
{
    const int Nmax = 5;
    int N;

    // read-in polynomial degree
    printf("\n Input polynomial degree (0-%i): ", Nmax);
    if (scanf("%i", &N) != 1) {
        printf(" Failed to read N.\n\n");
        exit(1);
    }
    if (N < 0 || N > Nmax) {
        printf(" Invalid value N = %i.\n", N);
        printf(" N must satisfy: 0 <= N <= %i\n\n", Nmax);
        exit(1);
    }
    printf("\n");

    // read-in coefficients
    double b[Nmax + 1];
    for (int i = 0; i <= N; i++) {
        printf(" Set b[%i]: ", i);
        if (scanf("%lf", &b[i]) != 1) {
            printf(" Failed to read b[%i].\n\n", i);
            exit(1);
        }
    }
    printf("\n");

    // set x-coordinates
    const int NumPts = 21;
    double x[NumPts];
    for (int i = 0; i < NumPts; i++) {
        x[i] = -1.0 + i * (2.0 / (1.0 * (NumPts - 1)));
    }

    // calculate polynomial at x-coordinates
    double y[NumPts];
    SamplePoly(N, NumPts, b, x, y);

    // write to lab9/poly.data (NO cd needed)
    WritePoly("lab9/poly.data", NumPts, x, y);

    // call python script using the venv interpreter in CURRENT folder
#ifdef _WIN32
    system(".\\.venv\\Scripts\\python lab9\\plot_chebyshev.py");
#else
    system("./.venv/bin/python lab9/plot_chebyshev.py");
#endif

    return 0;
}

static void SamplePoly(const int N,
                       const int NumPts,
                       const double b[],
                       const double x[],
                       double y[])
{
    for (int i = 0; i < NumPts; i++) {
        const double a = x[i];
        y[i] = b[0]; // phi_0 = 1

        for (int k = 1; k <= N; k++) {
            double phi;

            switch (k) {
                case 1:
                    phi = a;
                    break;
                case 2:
                    phi = 2.0 * a * a - 1.0;
                    break;
                case 3:
                    phi = 4.0 * a * a * a - 3.0 * a;
                    break;
                case 4:
                    phi = 8.0 * pow(a, 4.0) - 8.0 * a * a + 1.0;
                    break;
                case 5:
                    phi = 16.0 * pow(a, 5.0) - 20.0 * a * a * a + 5.0 * a;
                    break;
                default:
                    printf("\n Error: basis only implemented up to degree 5.\n\n");
                    exit(1);
            }

            y[i] += b[k] * phi;
        }
    }
}

static void WritePoly(const char* filepath,
                      const int NumPts,
                      const double x[],
                      const double y[])
{
    FILE *fid = fopen(filepath, "w");
    if (!fid) {
        printf(" Error: could not open %s for writing.\n\n", filepath);
        exit(1);
    }

    for (int i = 0; i < NumPts; i++) {
        fprintf(fid, "%.16e  %.16e\n", x[i], y[i]);
    }

    fclose(fid);
}


/*
powershell
gcc .\lab9\chebyshev.c -o .\lab9\chebyshev.exe
.\lab9\chebyshev.exe
*/

/*
bash
source .venv/bin/activate
gcc lab9/chebyshev.c -o lab9/chebyshev
./lab9/chebyshev

gcc lab9/chebyshev.c -lm -o chebyshev
./chebyshev
*/