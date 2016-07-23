#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

  int SUM, world_size, world_rank, Spartner_rank, Rpartner_rank, i;

  int A, B;

  int init = 1;

  MPI_Init(NULL, NULL);

  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  Spartner_rank = (world_rank + 1) % (world_size);
  Rpartner_rank = (world_rank - 1); if (Rpartner_rank < 0){Rpartner_rank = world_size - 1;}
  printf("WS: %d, MyRank: %d, SendP: %d, RecP: %d\n", world_size, world_rank, Spartner_rank, Rpartner_rank);

  MPI_Request req[2];
	MPI_Status stat[2];

  A = world_rank;
  SUM = 0;

  while (A != world_rank || init)
  {
    init = 0;

    MPI_Isend(&A, 1, MPI_INT, Spartner_rank, 0, MPI_COMM_WORLD, &req[0]);

    MPI_Irecv(&B, 1, MPI_INT, Rpartner_rank, 0, MPI_COMM_WORLD, &req[1]);

    MPI_Waitall(2, req, stat);
    //printf("Proccess %d sent to proccess %d A: %d and receive from proccess %d B: %d  \n", world_rank, Spartner_rank, A, Rpartner_rank, B);

    A = B;
    SUM += A;
  }

  printf("Hello from rank %d, reading SUM: %d  \n", world_rank, SUM);

  MPI_Finalize();
}
