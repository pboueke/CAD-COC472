#include "mpi.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//A X B = C

int main( int argc, char ** argv){

  int N, N2, world_rank, world_size, i, j, k, proccess_N;
  int *A, *B, *C, *proccess_A, *proccess_B, *proccess_C;

  if (argc > 1)
  {
    N = atoi(argv[1]);
  }
  else
  {
    printf("Invalid number of arguments\n");
    return 1;
  }

  N2 = N*N;

  // init MPI
  MPI_Init( NULL, NULL );
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  //hoping for the best (equal number of lines per proccess)
  proccess_N = N/world_size;

  if (world_rank == 0)
  {
    // initializing A and B for the root process
    A = malloc(N2*sizeof(int));
    B = malloc(N2*sizeof(int));

    for (i = 0; i < N2; i++)
    {
        A[i] = i;
        B[i] = i%1000;
    }
  }
  else
  {
    // allocating B for every process
    B = malloc(N2*sizeof(int));
  }

 //receive buffers
  proccess_A = malloc(proccess_N*N*sizeof(int));
  proccess_C = malloc(proccess_N*N*sizeof(int));

  // broadcast B and scatter A (ideally we should transpose B and then scatter it)
  MPI_Bcast(B, N2, MPI_INTEGER, 0, MPI_COMM_WORLD);
  MPI_Scatter(A, proccess_N*N, MPI_INTEGER, proccess_A, proccess_N*N, MPI_INTEGER, 0, MPI_COMM_WORLD);

  // calculating
  for (i = 0; i < proccess_N; i++)
  {
    for (j = 0; j < N; j++)
    {
      proccess_C[(i*N) + j] = 0;
      for (k = 0; k < N; k++)
      {
        proccess_C[(i*N) + j] += proccess_A[i*N+k]*B[k*N+j];
      }
    }
  }

  //allocating final matrix for root
  if (world_rank == 0)
  {
    C = malloc(N2*sizeof(int));
  }

  // gathering results from processes to master
  MPI_Gather(proccess_C, proccess_N*N, MPI_INTEGER, C, proccess_N*N, MPI_INTEGER, 0, MPI_COMM_WORLD);

  if (world_rank == 0)
  {
    printf("Finhshed\n");
  }

  // free everything
  free(proccess_A);
  free(proccess_C);
  
  free(B);
  if (world_rank == 0)
  {
    free(A);
    free(C);
  }

  MPI_Finalize();
}
