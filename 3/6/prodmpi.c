#include "mpi.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char ** argv){

  int N, world_rank, world_size, i, proccess_N;
  int *A, *B, *proccess_A, *proccess_B;
  int globalSum, sum = 0;

  if (argc > 1)
  {
    N = atoi(argv[1]);
  }
  else
  {
    printf("Invalid number of arguments\n");
  }

  // init MPI
  MPI_Init( NULL, NULL );
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  char outfilename[40];
  sprintf(outfilename, "found.data_%d", world_rank);
  FILE* outfile = fopen(outfilename, "w");
  fprintf(outfile, "\n\nProcess %d initialized of %d\n", world_rank, world_size);
  fflush(outfile);

  //hoping for the best (equal number of lines per proccess)
  proccess_N = N/world_size;

  if (world_rank == 0)
  {
    // initializing A and B for the root process
    A = malloc(N*sizeof(int));
    B = malloc(N*sizeof(int));

    for (i = 0; i < N; i++)
    {
        A[i] = i;
        B[i] = i;
    }
  }

 //receive buffers
  proccess_A = malloc(proccess_N*sizeof(int));
  proccess_B = malloc(proccess_N*sizeof(int));
  fprintf(outfile, "\n\nProcess array allocated", world_rank);
  fflush(outfile);

  // scatter B and scatter A
  MPI_Scatter(A, proccess_N, MPI_INTEGER, proccess_A, proccess_N, MPI_INTEGER, 0, MPI_COMM_WORLD);
  MPI_Scatter(B, proccess_N, MPI_INTEGER, proccess_B, proccess_N, MPI_INTEGER, 0, MPI_COMM_WORLD);

  fprintf(outfile, "\n\nProcess %d first elements of  local A and  local B %d %d\n", world_rank, proccess_A[0], proccess_B[0]);
  fflush(outfile);
  // calculating
  for (i = 0; i < proccess_N; i++)
  {
      sum += proccess_A[i] * proccess_B[i];

  }
  fprintf(outfile, "process %d, sum %d \n", world_rank, sum);
  fflush(outfile);

  // gathering results from processes to master
  MPI_Reduce(&sum, &globalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (world_rank == 0)
  {
    printf("Finished %d\n", globalSum);
  }

  // free everything
  free(proccess_A);
  free(proccess_B);
  if (world_rank == 0){
    free(A);
    free(B);
  }

  MPI_Finalize();
}
