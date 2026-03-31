#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

static void init_signal(double *x, long long N, int thread_count)
{
    #pragma omp parallel for num_threads(thread_count) schedule(static)
    for (long long i = 0; i < N; ++i)
    {
        long long k = (17LL * i + 13LL) % 1000LL;
        x[i] = ((double)k - 500.0) / 500.0;
    }
}

static double check_norm2(const double *x, long long N, int thread_count)
{
    double sumsq = 0.0;

    #pragma omp parallel for num_threads(thread_count) reduction(+:sumsq) schedule(static)
    for (long long i = 0; i < N; ++i)
    {
        sumsq += x[i] * x[i];
    }

    return sqrt(sumsq);
}

static double check_maxnorm(const double *x, long long N, int thread_count)
{
    double maxabs = 0.0;

    #pragma omp parallel for num_threads(thread_count) reduction(max:maxabs) schedule(static)
    for (long long i = 0; i < N; ++i)
    {
        double a = fabs(x[i]);
        if (a > maxabs) maxabs = a;
    }

    return maxabs;
}

/* -------------------- Fine-grain versions -------------------- */

static double normalize_2norm_fine(double *x, long long N, int thread_count, double *norm_before)
{
    double t1 = omp_get_wtime();

    double sumsq = 0.0;
    #pragma omp parallel for num_threads(thread_count) reduction(+:sumsq) schedule(static)
    for (long long i = 0; i < N; ++i)
    {
        sumsq += x[i] * x[i];
    }

    double norm2 = sqrt(sumsq);
    *norm_before = norm2;

    #pragma omp parallel for num_threads(thread_count) schedule(static)
    for (long long i = 0; i < N; ++i)
    {
        x[i] /= norm2;
    }

    double t2 = omp_get_wtime();
    return t2 - t1;
}

static double normalize_maxnorm_fine(double *x, long long N, int thread_count, double *norm_before)
{
    double t1 = omp_get_wtime();

    double maxabs = 0.0;
    #pragma omp parallel for num_threads(thread_count) reduction(max:maxabs) schedule(static)
    for (long long i = 0; i < N; ++i)
    {
        double a = fabs(x[i]);
        if (a > maxabs) maxabs = a;
    }

    *norm_before = maxabs;

    #pragma omp parallel for num_threads(thread_count) schedule(static)
    for (long long i = 0; i < N; ++i)
    {
        x[i] /= maxabs;
    }

    double t2 = omp_get_wtime();
    return t2 - t1;
}

/* -------------------- Coarse-grain versions -------------------- */
/* Manual block decomposition per thread */

static double normalize_2norm_coarse(double *x, long long N, int thread_count, double *norm_before)
{
    double *partial_sums = (double *)calloc((size_t)thread_count, sizeof(double));
    if (partial_sums == NULL)
    {
        fprintf(stderr, "Error: could not allocate partial_sums.\n");
        exit(EXIT_FAILURE);
    }

    double norm2 = 0.0;
    double t1 = omp_get_wtime();

    #pragma omp parallel num_threads(thread_count) shared(partial_sums, norm2)
    {
        int tid = omp_get_thread_num();
        long long start = (N * tid) / thread_count;
        long long end   = (N * (tid + 1)) / thread_count;

        double local_sum = 0.0;
        for (long long i = start; i < end; ++i)
        {
            local_sum += x[i] * x[i];
        }
        partial_sums[tid] = local_sum;

        #pragma omp barrier

        #pragma omp single
        {
            double total = 0.0;
            for (int t = 0; t < thread_count; ++t)
            {
                total += partial_sums[t];
            }
            norm2 = sqrt(total);
        }

        #pragma omp barrier

        for (long long i = start; i < end; ++i)
        {
            x[i] /= norm2;
        }
    }

    double t2 = omp_get_wtime();
    *norm_before = norm2;

    free(partial_sums);
    return t2 - t1;
}

static double normalize_maxnorm_coarse(double *x, long long N, int thread_count, double *norm_before)
{
    double *partial_max = (double *)calloc((size_t)thread_count, sizeof(double));
    if (partial_max == NULL)
    {
        fprintf(stderr, "Error: could not allocate partial_max.\n");
        exit(EXIT_FAILURE);
    }

    double maxabs = 0.0;
    double t1 = omp_get_wtime();

    #pragma omp parallel num_threads(thread_count) shared(partial_max, maxabs)
    {
        int tid = omp_get_thread_num();
        long long start = (N * tid) / thread_count;
        long long end   = (N * (tid + 1)) / thread_count;

        double local_max = 0.0;
        for (long long i = start; i < end; ++i)
        {
            double a = fabs(x[i]);
            if (a > local_max) local_max = a;
        }
        partial_max[tid] = local_max;

        #pragma omp barrier

        #pragma omp single
        {
            double total_max = 0.0;
            for (int t = 0; t < thread_count; ++t)
            {
                if (partial_max[t] > total_max) total_max = partial_max[t];
            }
            maxabs = total_max;
        }

        #pragma omp barrier

        for (long long i = start; i < end; ++i)
        {
            x[i] /= maxabs;
        }
    }

    double t2 = omp_get_wtime();
    *norm_before = maxabs;

    free(partial_max);
    return t2 - t1;
}

static void print_header(const char *title)
{
    printf("\n============================================================\n");
    printf("%s\n", title);
    printf("============================================================\n");
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <thread_count> <N>\n", argv[0]);
        fprintf(stderr, "Example: %s 8 100000000\n", argv[0]);
        return EXIT_FAILURE;
    }

    int thread_count = atoi(argv[1]);
    long long N = atoll(argv[2]);

    if (thread_count <= 0 || N <= 0)
    {
        fprintf(stderr, "Error: thread_count and N must be positive.\n");
        return EXIT_FAILURE;
    }

    size_t bytes = (size_t)N * sizeof(double);
    double gb = (double)bytes / (1024.0 * 1024.0 * 1024.0);

    double *x = (double *)malloc(bytes);
    if (x == NULL)
    {
        fprintf(stderr, "Error: could not allocate x (%.3f GB).\n", gb);
        return EXIT_FAILURE;
    }

    int actual_threads = 0;
    #pragma omp parallel num_threads(thread_count)
    {
        #pragma omp single
        actual_threads = omp_get_num_threads();
    }

    printf("Lab 19: Signal normalization with OpenMP\n");
    printf("Requested threads = %d\n", thread_count);
    printf("Actual threads    = %d\n", actual_threads);
    printf("N                 = %lld\n", N);
    printf("Memory for x      = %.3f GB\n", gb);

    /* Fine-grain 2-norm */
    init_signal(x, N, thread_count);
    double norm_before = 0.0;
    double time_used = normalize_2norm_fine(x, N, thread_count, &norm_before);
    double norm_after = check_norm2(x, N, thread_count);

    print_header("Fine-grain: 2-norm normalization");
    printf("2-norm before normalization = %.12e\n", norm_before);
    printf("2-norm after normalization  = %.12e\n", norm_after);
    printf("Elapsed time                = %.6f s\n", time_used);

    /* Fine-grain max norm */
    init_signal(x, N, thread_count);
    norm_before = 0.0;
    time_used = normalize_maxnorm_fine(x, N, thread_count, &norm_before);
    double max_after = check_maxnorm(x, N, thread_count);

    print_header("Fine-grain: max-norm normalization");
    printf("Max norm before normalization = %.12e\n", norm_before);
    printf("Max norm after normalization  = %.12e\n", max_after);
    printf("Elapsed time                  = %.6f s\n", time_used);

    /* Coarse-grain 2-norm */
    init_signal(x, N, thread_count);
    norm_before = 0.0;
    time_used = normalize_2norm_coarse(x, N, thread_count, &norm_before);
    norm_after = check_norm2(x, N, thread_count);

    print_header("Coarse-grain: 2-norm normalization");
    printf("2-norm before normalization = %.12e\n", norm_before);
    printf("2-norm after normalization  = %.12e\n", norm_after);
    printf("Elapsed time                = %.6f s\n", time_used);

    /* Coarse-grain max norm */
    init_signal(x, N, thread_count);
    norm_before = 0.0;
    time_used = normalize_maxnorm_coarse(x, N, thread_count, &norm_before);
    max_after = check_maxnorm(x, N, thread_count);

    print_header("Coarse-grain: max-norm normalization");
    printf("Max norm before normalization = %.12e\n", norm_before);
    printf("Max norm after normalization  = %.12e\n", max_after);
    printf("Elapsed time                  = %.6f s\n", time_used);

    free(x);
    return EXIT_SUCCESS;
}