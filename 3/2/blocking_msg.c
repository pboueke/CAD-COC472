#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);

  int partner_rank;

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  if (world_rank < world_size/2){
    partner_rank = world_size/2 + world_rank;
  }
  else if (world_rank >= world_size/2) {
    partner_rank = world_rank - world_size/2;
  }

  int out;
  // the lowest task sends first
  if (world_rank < partner_rank) {
    MPI_Send(&world_rank, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
  }
  else {
    MPI_Recv(&out, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Hello from rank %d, reading from rank %d \n", world_rank, out);
  }
  if (world_rank > partner_rank) {
    MPI_Send(&world_rank, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
  }
  else {
    MPI_Recv(&out, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Hello from rank %d, reading from rank %d \n", world_rank, out);
  }


  MPI_Finalize();
}
