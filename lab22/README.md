# Build both executables
make all

# Or individually
make mpi
make omp

make clean

# MPI
mpirun -np 4 ./monte_carlo_mpi 100000000

# OpenMp
OMP_NUM_THREADS=4 ./monte_carlo_omp 100000000
