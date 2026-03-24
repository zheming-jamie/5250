#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <threads> <N>\n", prog);
    fprintf(stderr, "  threads: number of OpenMP threads (e.g. 1, 2, 4, 8)\n");
    fprintf(stderr, "  N      : image size, must be >= 3\n");
    exit(EXIT_FAILURE);
}

static unsigned char *alloc_image(int rows, int cols) {
    unsigned char *img = (unsigned char *)malloc((size_t)rows * cols * sizeof(unsigned char));
    if (!img) {
        fprintf(stderr, "Error: memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return img;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        usage(argv[0]);
    }

    int thread_count = (int)strtol(argv[1], NULL, 10);
    int N = (int)strtol(argv[2], NULL, 10);

    if (thread_count <= 0 || N < 3) {
        usage(argv[0]);
    }

    const int outN = N - 2;

    unsigned char *img = alloc_image(N, N);
    unsigned char *blur = alloc_image(outN, outN);

    /* fixed seed for reproducibility */
    srand(42);

    /* generate random image: img[i][j] = rand() % 256 */
    for (int i = 0; i < N * N; i++) {
        img[i] = (unsigned char)(rand() % 256);
    }

    double t1 = omp_get_wtime();

    #pragma omp parallel for num_threads(thread_count) collapse(2)
    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < N - 1; j++) {
            int sum = 0;
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    sum += img[(i + di) * N + (j + dj)];
                }
            }
            blur[(i - 1) * outN + (j - 1)] = (unsigned char)(sum / 9);
        }
    }

    double t2 = omp_get_wtime();
    double walltime = t2 - t1;

    /* checksum to verify computation and avoid suspicious optimization */
    unsigned long long checksum = 0ULL;
    for (int i = 0; i < outN * outN; i++) {
        checksum += blur[i];
    }

    printf("Threads   : %d\n", thread_count);
    printf("Image size: %d x %d\n", N, N);
    printf("Blur size : %d x %d\n", outN, outN);
    printf("Walltime  : %.8f sec\n", walltime);
    printf("Checksum  : %llu\n", checksum);

    free(img);
    free(blur);
    return 0;
}