#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

  int size, world_size, world_rank, Spartner_rank, Rpartner_rank, i;

  size = 1000;

  float A[size], B[size];

  MPI_Init(NULL, NULL);

  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  Spartner_rank = (world_rank + 1) % (world_size);
  Rpartner_rank = (world_rank - 1); if (Rpartner_rank < 0){Rpartner_rank = world_size - 1;}
  printf("WS: %d, MyRank: %d, SendP: %d, RecP: %d\n", world_size, world_rank, Spartner_rank, Rpartner_rank);

  MPI_Request req[2];
	MPI_Status stat[2];

  //A = (float*)malloc(size);
  //B = (float*)malloc(size);

  for (i = 0; i < size; i += 1)
  {
    A[i] = (float)world_rank;
  }

  MPI_Isend(A, size, MPI_INT, Spartner_rank, 0, MPI_COMM_WORLD, &req[0]);
  printf("Proccess %d sent to proccess %d A[100]: %f  \n", world_rank, Spartner_rank, A[100]);

  MPI_Irecv(&B, size, MPI_INT, Rpartner_rank, 0, MPI_COMM_WORLD, &req[1]);

  MPI_Waitall(2, req, stat);

  printf("Hello from rank %d, reading from %d value of B[100]: %f \n", world_rank, Rpartner_rank, B[100]);

  MPI_Finalize();
}
