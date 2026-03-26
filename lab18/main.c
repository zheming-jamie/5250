#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static void usage(const char *prog_name);
static double normal_cdf(double x);
static double bs_call_price(double S0, double K, double r, double T, double sigma);
static double lognormal_pdf(double S, double S0, double r, double T, double sigma);
static double payoff_integrand(double S, double S0, double K, double r, double T, double sigma);
static double simpson_call_price_omp(double S0, double K, double r, double T,
                                     double sigma, double Smax, long long N,
                                     int thread_count);

int main(int argc, char *argv[])
{
    if (argc != 3) {
        usage(argv[0]);
    }

    int thread_count = (int)strtol(argv[1], NULL, 10);
    long long N = strtoll(argv[2], NULL, 10);

    if (thread_count < 1 || N < 2 || (N % 2 != 0)) {
        usage(argv[0]);
    }

    /* Test case from the assignment */
    const double S0 = 100.0;
    const double K = 100.0;
    const double r = 0.05;
    const double T = 1.0;
    const double sigma = 0.2;
    const double Smax = 5.0 * S0;

    double t1 = 0.0, t2 = 0.0;
#ifdef _OPENMP
    t1 = omp_get_wtime();
#endif

    double C_num = simpson_call_price_omp(S0, K, r, T, sigma, Smax, N, thread_count);

#ifdef _OPENMP
    t2 = omp_get_wtime();
#endif

    double C_exact = bs_call_price(S0, K, r, T, sigma);
    double abs_err = fabs(C_num - C_exact);
    double rel_err = abs_err / fabs(C_exact);

    printf("European Call Option Pricing by Simpson + OpenMP\n");
    printf("------------------------------------------------\n");
    printf("Parameters:\n");
    printf("S0 = %.6f, K = %.6f, r = %.6f, T = %.6f, sigma = %.6f\n", S0, K, r, T, sigma);
    printf("Smax = %.6f, N = %lld, threads = %d\n\n", Smax, N, thread_count);

    printf("Numerical Price     = %.15f\n", C_num);
    printf("Black-Scholes Price = %.15f\n", C_exact);
    printf("Absolute Error      = %.15e\n", abs_err);
    printf("Relative Error      = %.15e\n", rel_err);

#ifdef _OPENMP
    printf("Wall Time           = %.15e seconds\n", t2 - t1);
#endif

    return 0;
}

static void usage(const char *prog_name)
{
    fprintf(stderr, "usage: %s <num_threads> <N>\n", prog_name);
    fprintf(stderr, "  <num_threads> must be positive\n");
    fprintf(stderr, "  <N> must be a positive even integer (Simpson's rule)\n");
    exit(1);
}

static double normal_cdf(double x)
{
    return 0.5 * (1.0 + erf(x / sqrt(2.0)));
}

static double bs_call_price(double S0, double K, double r, double T, double sigma)
{
    double sqrtT = sqrt(T);
    double d1 = (log(S0 / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrtT);
    double d2 = d1 - sigma * sqrtT;

    return S0 * normal_cdf(d1) - K * exp(-r * T) * normal_cdf(d2);
}

static double lognormal_pdf(double S, double S0, double r, double T, double sigma)
{
    if (S <= 0.0) return 0.0;

    double mu = log(S0) + (r - 0.5 * sigma * sigma) * T;
    double z = (log(S) - mu) / (sigma * sqrt(T));

    return exp(-0.5 * z * z) / (S * sigma * sqrt(2.0 * M_PI * T));
}

static double payoff_integrand(double S, double S0, double K, double r, double T, double sigma)
{
    if (S <= K) return 0.0;
    return exp(-r * T) * (S - K) * lognormal_pdf(S, S0, r, T, sigma);
}

static double simpson_call_price_omp(double S0, double K, double r, double T,
                                     double sigma, double Smax, long long N,
                                     int thread_count)
{
    /* Integral over [K, Smax] */
    const double a = K;
    const double b = Smax;
    const double h = (b - a) / (double)N;

    double fa = payoff_integrand(a, S0, K, r, T, sigma);
    double fb = payoff_integrand(b, S0, K, r, T, sigma);

    double odd_sum = 0.0;
    double even_sum = 0.0;

    #pragma omp parallel for num_threads(thread_count) reduction(+:odd_sum)
    for (long long i = 1; i < N; i += 2) {
        double S = a + i * h;
        odd_sum += payoff_integrand(S, S0, K, r, T, sigma);
    }

    #pragma omp parallel for num_threads(thread_count) reduction(+:even_sum)
    for (long long i = 2; i < N; i += 2) {
        double S = a + i * h;
        even_sum += payoff_integrand(S, S0, K, r, T, sigma);
    }

    return (h / 3.0) * (fa + fb + 4.0 * odd_sum + 2.0 * even_sum);
}