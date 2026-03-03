#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846

double generate_normal(double sigma) {
    // Generate two uniform random numbers between 0 and 1
    // We use (double)rand() / RAND_MAX
    double u1 = (double)rand() / (double)RAND_MAX;
    double u2 = (double)rand() / (double)RAND_MAX;

    // Avoid log(0)
    if (u1 <= 0.0) u1 = 1e-10;

    // Box-Muller Transform for Z ~ N(0, 1)
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2);

    // Scale by sigma to get N(0, sigma^2)
    return z0 * sigma;
}

int normal_distribution_main() {
    srand(time(NULL)); // Seed the random number generator
    
    double sigma = 2.0;
    for(int i = 0; i < 10; i++) {
        printf("%f\n", generate_normal(sigma));
    }
    
    return 0;
}
