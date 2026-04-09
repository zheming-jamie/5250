# ============================================================
#  Makefile — Monte Carlo Pi: MPI & OpenMP implementations
#  Lab 22 — Com S / Cpr E / Math 5250
# ============================================================

CC      = gcc
MPICC   = mpicc

CFLAGS  = -O2 -Wall
OMPFLAGS = -fopenmp
LIBS    = -lm

MPI_SRC = monte_carlo_mpi.c
OMP_SRC = monte_carlo_omp.c

MPI_EXE = monte_carlo_mpi
OMP_EXE = monte_carlo_omp

# Default: build both
.PHONY: all mpi omp clean

all: touch mpi omp

touch:
	@touch $(MPI_SRC) $(OMP_SRC)

mpi: $(MPI_SRC)
	$(MPICC) $(CFLAGS) -o $(MPI_EXE) $(MPI_SRC) $(LIBS)
	@echo "Built $(MPI_EXE)"

omp: $(OMP_SRC)
	$(CC) $(CFLAGS) $(OMPFLAGS) -o $(OMP_EXE) $(OMP_SRC) $(LIBS)
	@echo "Built $(OMP_EXE)"

clean:
	rm -f $(MPI_EXE) $(OMP_EXE)
