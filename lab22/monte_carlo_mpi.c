/*
 * Monte Carlo estimation of Pi using MPI (coarse-grain parallelism).
 *
 * Strategy:
 *   - Each MPI rank independently generates N_local random points in [0,1]^2.
 *   - It counts how many fall inside the unit-circle quadrant (x^2+y^2 <= 1).
 *   - MPI_Reduce sums all local counts to rank 0, which prints pi ~ 4*count/N_total.
 *
 * Each rank seeds its RNG with (seed + rank) to ensure independent streams.
 *
 * Compile:  mpicc -O2 -o monte_carlo_mpi monte_carlo_mpi.c -lm
 * Run:      mpirun -np 4 ./monte_carlo_mpi 100000000
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* ---- parse total sample count ---- */
    if (argc != 2) {
        if (rank == 0)
            fprintf(stderr, "Usage: %s <N_total>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }
    long long N_total = atoll(argv[1]);
    if (N_total <= 0) {
        if (rank == 0)
            fprintf(stderr, "N_total must be positive.\n");
        MPI_Finalize();
        return 1;
    }

    /* ---- divide work evenly; last rank gets the remainder ---- */
    long long N_local = N_total / size;
    if (rank == size - 1)
        N_local += N_total % size;

    /* ---- seed RNG uniquely per rank ---- */
    /* Multiply rank+1 by a large prime so seeds are well-separated,
     * not just consecutive integers.  XOR with wall-clock time so
     * repeated runs also differ.                                     */
    unsigned int seed = (unsigned int)((rank + 1) * 1000003U)
                        ^ (unsigned int)time(NULL);
    srand(seed);

    /* Print each rank's seed so the assignment can be verified */
    printf("  [rank %d] seed = %u, N_local = %lld\n", rank, seed, N_local);

    /* ---- Monte Carlo loop ---- */
    long long inside = 0;
    for (long long i = 0; i < N_local; i++) {
        double x = (double)rand() / (double)RAND_MAX;
        double y = (double)rand() / (double)RAND_MAX;
        if (x * x + y * y <= 1.0)
            inside++;
    }

    /* ---- reduce across all ranks ---- */
    long long inside_total = 0;
    long long N_total_actual = 0;

    MPI_Reduce(&inside,  &inside_total,   1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&N_local, &N_total_actual, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi_approx = 4.0 * (double)inside_total / (double)N_total_actual;
        printf("=== MPI Monte Carlo Pi ===\n");
        printf("  MPI ranks     : %d\n", size);
        printf("  Total samples : %lld\n", N_total_actual);
        printf("  Points inside : %lld\n", inside_total);
        printf("  Pi estimate   : %.10f\n", pi_approx);
        printf("  Pi (exact)    : %.10f\n", M_PI);
        printf("  Error         : %.2e\n", fabs(pi_approx - M_PI));
    }

    MPI_Finalize();
    return 0;
}
