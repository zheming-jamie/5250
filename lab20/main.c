#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static int g_case_id = 0;
static double g_param = 0.0;
static const char* g_logfile = "quadrature.data";

static void usage(const char* prog_name) {
    fprintf(stderr,
            "usage: %s <case_id> <num_threads> <TOL> <param> [logfile]\n"
            "  case_id = 1 for electrostatic potential V(x0) on [-1,1]\n"
            "            2 for Bessel J0(x) via theta-integral on [0,pi]\n"
            "  num_threads > 0\n"
            "  TOL > 5e-16\n"
            "  param = x0 for case 1, x for case 2\n",
            prog_name);
    exit(EXIT_FAILURE);
}

static inline double rho_case1(const double x) {
    return exp(-(x * x));
}

static double integrand(const double x) {
    if (g_case_id == 1) {
        const double x0 = g_param;
        const double diff = x - x0;
        if (fabs(diff) < 1.0e-14) {
            return 0.0;
        }
        return (rho_case1(x) - rho_case1(x0)) / fabs(diff);
    }
    if (g_case_id == 2) {
        return cos(g_param * sin(x)) / M_PI;
    }
    return 0.0;
}

static double Q(const double a, const double b) {
    const double c = 0.5 * (a + b);
    const double one_sixth = 1.0 / 6.0;
    return one_sixth * (b - a) * (integrand(a) + 4.0 * integrand(c) + integrand(b));
}

static void log_interval(const char* filename, const double a, const double b) {
    FILE* file = NULL;
    const int my_rank = omp_get_thread_num();
    #pragma omp critical(logfile_write)
    {
        file = fopen(filename, "a");
        if (file != NULL) {
            fprintf(file, "%3i %24.15e %24.15e\n", my_rank + 1, a, b);
            fclose(file);
        }
    }
}

static double AdaptiveIntSerial(const double a, const double b, const double TOL, const char* filename) {
    const double Qab = Q(a, b);
    const double c = 0.5 * (a + b);
    const double Qac = Q(a, c);
    const double Qcb = Q(c, b);
    const double one_over_15 = 1.0 / 15.0;
    const double error_est = one_over_15 * fabs(Qac + Qcb - Qab);

    log_interval(filename, a, b);

    if (error_est < TOL) {
        return Qac + Qcb;
    }
    return AdaptiveIntSerial(a, c, 0.5 * TOL, filename) +
           AdaptiveIntSerial(c, b, 0.5 * TOL, filename);
}

static double AdaptiveInt(const double a,
                          const double b,
                          const double TOL,
                          const char* filename,
                          const int num_threads) {
    const double Qab = Q(a, b);
    const double c = 0.5 * (a + b);
    const double Qac = Q(a, c);
    const double Qcb = Q(c, b);
    const double one_over_15 = 1.0 / 15.0;
    const double error_est = one_over_15 * fabs(Qac + Qcb - Qab);

    log_interval(filename, a, b);

    if (error_est < TOL || num_threads <= 1) {
        if (error_est < TOL) {
            return Qac + Qcb;
        }
        return AdaptiveIntSerial(a, b, TOL, filename);
    }

    double result = 0.0;
    const double one_over = 1.0 / ((double) num_threads);

    #pragma omp parallel num_threads(num_threads)
    {
        const int my_rank_local = omp_get_thread_num();
        const double a_local = a + my_rank_local * (b - a) * one_over;
        const double b_local = a_local + (b - a) * one_over;
        const double local_result = AdaptiveInt(a_local, b_local, one_over * TOL, filename, num_threads);
        #pragma omp critical(sum_result)
        result += local_result;
    }

    return result;
}

static double solve_case1(const int num_threads, const double TOL, const char* filename) {
    const double a = -1.0;
    const double b = 1.0;
    const double x0 = g_param;
    if (!(x0 >= a && x0 <= b)) {
        fprintf(stderr, "case 1 requires x0 in [-1,1].\n");
        exit(EXIT_FAILURE);
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fclose(file);

    double regularized = 0.0;
    if (num_threads == 1) {
        regularized = AdaptiveIntSerial(a, b, TOL, filename);
    } else {
        regularized = AdaptiveInt(a, b, TOL, filename, num_threads);
    }

    const double analytic_term = rho_case1(x0) * log((b - x0) / (x0 - a));
    return regularized + analytic_term;
}

static double solve_case2(const int num_threads, const double TOL, const char* filename) {
    const double a = 0.0;
    const double b = M_PI;

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fclose(file);

    if (num_threads == 1) {
        return AdaptiveIntSerial(a, b, TOL, filename);
    }
    return AdaptiveInt(a, b, TOL, filename, num_threads);
}

int main(int argc, char* argv[]) {
    if (argc < 5 || argc > 6) {
        usage(argv[0]);
    }

    g_case_id = (int) strtol(argv[1], NULL, 10);
    const int num_threads = (int) strtol(argv[2], NULL, 10);
    const double TOL = strtod(argv[3], NULL);
    g_param = strtod(argv[4], NULL);
    if (argc == 6) {
        g_logfile = argv[5];
    }

    if ((g_case_id != 1 && g_case_id != 2) || num_threads <= 0 || TOL < 5.0e-16) {
        usage(argv[0]);
    }

    omp_set_dynamic(0);
    omp_set_max_active_levels(omp_get_supported_active_levels());

    const double t0 = omp_get_wtime();
    double value = 0.0;
    if (g_case_id == 1) {
        value = solve_case1(num_threads, TOL, g_logfile);
    } else {
        value = solve_case2(num_threads, TOL, g_logfile);
    }
    const double t1 = omp_get_wtime();

    printf("case_id = %d\n", g_case_id);
    printf("thread_count = %d\n", num_threads);
    printf("TOL = %.15e\n", TOL);
    if (g_case_id == 1) {
        printf("x0 = %.15e\n", g_param);
        printf("V(x0) = %.15e\n", value);
    } else {
        printf("x = %.15e\n", g_param);
        printf("J0(x) = %.15e\n", value);
    }
    printf("wall_time = %.15e\n", t1 - t0);
    printf("log_file = %s\n", g_logfile);

    return 0;
}
