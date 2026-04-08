#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(void)
{
    int my_rank, comm_sz;
    int token;
    const int tag = 0;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (comm_sz < 2)
    {
        if (my_rank == 0)
        {
            fprintf(stderr, "This program requires at least 2 MPI processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (my_rank == 0)
    {
        token = 1000;

        MPI_Send(&token, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
        printf("Rank %d sent token %d to Rank %d\n", my_rank, token, 1);

        MPI_Recv(&token, 1, MPI_INT, comm_sz - 1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank %d received token %d from Rank %d\n", my_rank, token, comm_sz - 1);
        printf("Final token value after full trip: %d\n", token);
    }
    else
    {
        int source = my_rank - 1;
        int dest = (my_rank + 1) % comm_sz;

        MPI_Recv(&token, 1, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank %d received token %d from Rank %d\n", my_rank, token, source);

        token += 1;

        MPI_Send(&token, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
        printf("Rank %d sent token %d to Rank %d\n", my_rank, token, dest);
    }

    MPI_Finalize();
    return 0;
}
