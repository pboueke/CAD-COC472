#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NI 700        /* tamanho dos array  */
#define NJ 700

#define NSTEPS 500    /* Numero de iteracoes */

int main(int argc, char *argv[])
{

  int i, j, n, im, ip, jm, jp, ni, nj, nsum, isum, world_size, world_rank, chunk_size, live_ni, live_nj, next_rank, previous_rank, distributed_sum;
  int **old, **new;
  float x;

  MPI_Init(NULL, NULL);

  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  if (world_rank == 0)
  {
    printf("Total number of process: %d \n",  world_size);
  }
  char outfilename[40];
  sprintf(outfilename, "found.data_%d", world_rank);
  FILE* outfile = fopen(outfilename, "w");
  fprintf(outfile, "\n\nProcess %d initialized\n", world_rank);
  fflush(outfile);

  // todos os processos recebem (floor(NI / world_size) + 2) linhas
  // ecxeto ultimo processo, que receberá todo o restante

  chunk_size = NI / world_size;

  /* alocacao */

  ni = (chunk_size) + 2;  /* celulas fantasmas na borda  */
  nj = NJ + 2;
  if (world_rank == world_size -1)
  {
    ni = NI + 2 - ((world_size-1)*chunk_size);
  }
  live_ni = ni - 2;
  live_nj = nj - 2;

  old = malloc(ni*sizeof(int*));
  new = malloc(ni*sizeof(int*));

  for(i=0; i<ni; i++){
    old[i] = malloc(nj*sizeof(int));
    new[i] = malloc(nj*sizeof(int));
  }

  fprintf(outfile, "Memory allocated \n");
	fflush(outfile);

/*  inicializando elementos  */

  for(i=1; i<=live_ni; i++)
  {
    for(j=1; j<=live_nj; j++)
    {
       x = rand()/((float)RAND_MAX + 1);
       if(x<0.5){
	  old[i][j] = 0;
       } else {
	  old[i][j] = 1;
       }
    }
  }

  next_rank = (world_rank < world_size - 1) ? world_rank + 1 : 0;
  previous_rank = (world_rank > 0) ? world_rank - 1 : world_size - 1;

  fprintf(outfile, "Data initialized \n");
	fflush(outfile);

  /* */
  for(n=0; n<NSTEPS; n++)
  {

    /* cond. contorno para faces direita/esquerda  */

    for(i=1; i<=live_ni; i++){
      old[i][0]    = old[i][live_nj];
      old[i][live_nj-1] = old[i][1];
    }

    /*Exchange data between cells*/
    MPI_Request req[4];
		MPI_Status stat[4];

    //up
    MPI_Isend(old[1], nj, MPI_INT, previous_rank, n, MPI_COMM_WORLD, &req[0]);
    MPI_Irecv(old[0], nj, MPI_INT, previous_rank, n, MPI_COMM_WORLD, &req[1]);
    //down
    MPI_Isend(old[live_ni], nj, MPI_INT, next_rank, n, MPI_COMM_WORLD, &req[2]);
    MPI_Irecv(old[live_ni + 1], nj, MPI_INT, next_rank, n, MPI_COMM_WORLD, &req[3]);

    if (n%100 == 0)
    {
      fprintf(outfile, "Iteration %d, all messages sent, continuing... \n", n);
      fflush(outfile);
    }

    /* calculating self-enclosed data */
    for(i=2; i<=ni-2; i++)
    {
      for(j=2; j<=nj; j++)
      {
        im = i-1;
        ip = i+1;
        jm = j-1;
        jp = j+1;

        nsum =  old[im][jp] + old[i][jp] + old[ip][jp]
              + old[im][j ]              + old[ip][j ]
              + old[im][jm] + old[i][jm] + old[ip][jm];

        switch(nsum)
        {
        case 3:
          new[i][j] = 1;
          break;
        case 2:
          new[i][j] = old[i][j];
          break;
        default:
          new[i][j] = 0;
        }
     }
   }

   /* wait all the messagesand then calculate the rest */
   MPI_Waitall(4, req, stat);

   if (n%100 == 0)
   {
     fprintf(outfile, "Iteration %d, received all messages, continuing... \n", n);
     fflush(outfile);
   }

   /* condicoes de controno para as esquinas do dominio */

    if (world_rank == 0)
    {
       old[0][0]    = old[live_ni][live_nj];
       old[0][nj-1] = old[live_ni][1];
    }
    if (world_rank == world_size - 1)
    {
      old[ni-1][nj-1] = old[1][1];
      old[ni-1][0]    = old[1][live_nj];
    }

   /* calculating data that depens on the messages*/
   for(i=1; i< live_ni + 1; i += (live_ni - 1))
   {
     // for i in (1, live_ni)
     for(j=2; j<=nj; j++)
     {
       im = i-1;
       ip = i+1;
       jm = j-1;
       jp = j+1;

       nsum =  old[im][jp] + old[i][jp] + old[ip][jp]
             + old[im][j ]              + old[ip][j ]
             + old[im][jm] + old[i][jm] + old[ip][jm];

       switch(nsum)
       {
       case 3:
         new[i][j] = 1;
         break;
       case 2:
         new[i][j] = old[i][j];
         break;
       default:
         new[i][j] = 0;
       }
    }
  }

    if (n%100 == 0)
    {
      fprintf(outfile, "Iteration %d, calculations done, continuing... \n", n);
      fflush(outfile);
    }

    /* copia estado  */
    for(i=1; i<=live_ni; i++)
    {
      for(j=1; j<=live_nj; j++)
      {
	       old[i][j] = new[i][j];
      }
    }
  }

  fprintf(outfile, "End of main loop. \n");
  fflush(outfile);

  /*  Conta o número de celulas  vivas no final */
  isum = 0;
  for(i=1; i<=live_ni; i++){
    for(j=1; j<=live_nj; j++){
      isum = isum + new[i][j];
    }
  }
  printf("Live cells for process [%d]: %d\n", world_rank, isum);
	fprintf(outfile, "Live cells for process [%d]: %d\n", world_rank, isum);
	fclose(outfile);

	MPI_Reduce(&isum, &distributed_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (world_rank == 0)
  {
		printf("Total live cells: %d\n", distributed_sum);
	}

  for(i=0; i<ni; i++){
    free(old[i]);
    free(new[i]);
  }

  free(old);
  free(new);

  MPI_Finalize();
}
