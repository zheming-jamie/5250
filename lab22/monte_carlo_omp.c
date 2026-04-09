/*
 * Monte Carlo estimation of Pi using OpenMP (coarse-grain parallelism).
 *
 * Strategy:
 *   - The total N samples are divided equally among OMP_NUM_THREADS threads
 *     via a parallel for with reduction on the inside counter.
 *   - Each thread uses its own thread-private seed to avoid lock contention
 *     on the RNG and to keep streams independent.
 *   - Pi ~ 4 * inside / N_total.
 *
 * Compile:  gcc -O2 -fopenmp -o monte_carlo_omp monte_carlo_omp.c -lm
 * Run:      OMP_NUM_THREADS=4 ./monte_carlo_omp 100000000
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    /* ---- parse total sample count ---- */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <N_total>\n", argv[0]);
        return 1;
    }
    long long N_total = atoll(argv[1]);
    if (N_total <= 0) {
        fprintf(stderr, "N_total must be positive.\n");
        return 1;
    }

    long long inside = 0;
    int nthreads = 0;

    /* ---- parallel region ---- */
    #pragma omp parallel reduction(+:inside)
    {
        int tid      = omp_get_thread_num();
        int nthr     = omp_get_num_threads();

        /* record thread count once */
        #pragma omp single
        nthreads = nthr;

        /* coarse-grain: each thread owns a contiguous chunk of N_total */
        long long chunk = N_total / nthr;
        long long start = (long long)tid * chunk;
        long long end   = (tid == nthr - 1) ? N_total : start + chunk;

        /* Unique seed per thread: multiply (tid+1) by a large prime so
         * seeds are well-separated, not just consecutive integers.
         * XOR with wall-clock time so repeated runs also differ.     */
        unsigned int seed = (unsigned int)((tid + 1) * 1000003U)
                            ^ (unsigned int)time(NULL);

        /* Print each thread's seed so the assignment can be verified */
        #pragma omp critical
        printf("  [thread %d] seed = %u, range [%lld, %lld)\n",
               tid, seed, start, end);

        long long local_inside = 0;
        for (long long i = start; i < end; i++) {
            /* rand_r is thread-safe when given a per-thread seed */
            double x = (double)rand_r(&seed) / (double)RAND_MAX;
            double y = (double)rand_r(&seed) / (double)RAND_MAX;
            if (x * x + y * y <= 1.0)
                local_inside++;
        }
        inside += local_inside;
    }

    double pi_approx = 4.0 * (double)inside / (double)N_total;
    printf("=== OpenMP Monte Carlo Pi ===\n");
    printf("  OMP threads   : %d\n", nthreads);
    printf("  Total samples : %lld\n", N_total);
    printf("  Points inside : %lld\n", inside);
    printf("  Pi estimate   : %.10f\n", pi_approx);
    printf("  Pi (exact)    : %.10f\n", M_PI);
    printf("  Error         : %.2e\n", fabs(pi_approx - M_PI));

    return 0;
}
