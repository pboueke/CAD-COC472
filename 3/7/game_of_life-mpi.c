#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NI 700        /* tamanho dos array  */
#define NJ 700

#define NSTEPS 500    /* Numero de iteracoes */

int main(int argc, char *argv[])
{
    MPI_Init(NULL, NULL);

    int i, j, n, im, ip, jm, jp, ni, nj, nsum, isum, world_size, world_rank, chunk_size, live_ni, live_nj, next_rank, previous_rank, distributed_sum;
    int **old_arr, **new_arr;
    float x;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    distributed_sum = 0;

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
        // last process takes the remainder lines
        ni = NI - ((world_size-1)*chunk_size) + 2;
    }
    // adjusted indexes for handling ghost cells
    live_ni = ni - 2;
    live_nj = nj - 2;

    /*old_arr = new int*[ni];
    new_arr = new int*[ni];

    for(i=0; i<ni; i++)
    {
        old_arr[i] = new int[nj];
        new_arr[i] = new int[nj];
    }*/

    // in process allocation of arrays
    old_arr = malloc(ni*sizeof(int*));
    new_arr = malloc(ni*sizeof(int*));

    for(i=0; i<ni; i++){
      old_arr[i] = malloc(nj*sizeof(int));
      new_arr[i] = malloc(nj*sizeof(int));
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
                old_arr[i][j] = 0;
            } else {
                old_arr[i][j] = 1;
            }
        }
    }

    // calculating neighbors rakns
    next_rank = (world_rank < world_size - 1) ? world_rank + 1 : 0;
    previous_rank = (world_rank > 0) ? world_rank - 1 : world_size - 1;

    fprintf(outfile, "Data initialized \n");
    fflush(outfile);

    /* game of life iterations */
    for(n=0; n<NSTEPS; n++)
    {

        /* cond. contorno para faces direita/esquerda  */

        for(i=1; i<=live_ni; i++){
            old_arr[i][0]    = old_arr[i][live_nj];
            old_arr[i][nj-1] = old_arr[i][1];
        }

        /*Exchange data between cells*/
        MPI_Request req[4];
        MPI_Status stat[4];

        //up neighbor
        MPI_Isend(old_arr[1], nj, MPI_INT, previous_rank, n, MPI_COMM_WORLD, &req[0]);
        MPI_Irecv(old_arr[0], nj, MPI_INT, previous_rank, n, MPI_COMM_WORLD, &req[1]);
        //down neighbor
        MPI_Isend(old_arr[live_ni], nj, MPI_INT, next_rank, n, MPI_COMM_WORLD, &req[2]);
        MPI_Irecv(old_arr[live_ni + 1], nj, MPI_INT, next_rank, n, MPI_COMM_WORLD, &req[3]);

        if (n%100 == 0)
        {
            fprintf(outfile, "Iteration %d, all messages sent, continuing... \n", n);
            fflush(outfile);
        }

        /* calculating self-enclosed data */
        for(i=2; i<=ni-2; i++)
        {
            for(j=1; j<=nj-1; j++)
            {
                im = i-1;
                ip = i+1;
                jm = j-1;
                jp = j+1;

                nsum =  old_arr[im][jp] + old_arr[i][jp] + old_arr[ip][jp]
                + old_arr[im][j ] + old_arr[ip][j ]
                + old_arr[im][jm] + old_arr[i][jm] + old_arr[ip][jm];

                switch(nsum)
                {
                    case 3:
                    new_arr[i][j] = 1;
                    break;
                    case 2:
                    new_arr[i][j] = old_arr[i][j];
                    break;
                    default:
                    new_arr[i][j] = 0;
                }
            }
        }

        /* wait all the messages and then calculate the rest (that depends on the received data)*/
        MPI_Waitall(4, req, stat);

        if (n%100 == 0)
        {
            fprintf(outfile, "Iteration %d, received all messages, continuing... \n", n);
            fflush(outfile);
        }

        /* condicoes de controno para as esquinas do dominio */

        if (world_rank == 0)
        {
            old_arr[0][0]    = old_arr[live_ni][live_nj];
            old_arr[0][nj-1] = old_arr[live_ni][1];
        }
        if (world_rank == world_size - 1)
        {
            old_arr[ni-1][nj-1] = old_arr[1][1];
            old_arr[ni-1][0]    = old_arr[1][live_nj];
        }

        /* calculating the rest */
        for(i=1; i< live_ni + 1; i += (live_ni - 1))
        {
            // for i in [1, live_ni]
            for(j=1; j<=nj-1; j++)
            {
                im = i-1;
                ip = i+1;
                jm = j-1;
                jp = j+1;

                nsum =  old_arr[im][jp] + old_arr[i][jp] + old_arr[ip][jp]
                + old_arr[im][j ] + old_arr[ip][j ]
                + old_arr[im][jm] + old_arr[i][jm] + old_arr[ip][jm];

                switch(nsum)
                {
                    case 3:
                    new_arr[i][j] = 1;
                    break;
                    case 2:
                    new_arr[i][j] = old_arr[i][j];
                    break;
                    default:
                    new_arr[i][j] = 0;
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
                old_arr[i][j] = new_arr[i][j];
            }
        }
    }

    fprintf(outfile, "End of main loop. \n");
    fflush(outfile);

    /*  Conta o número de celulas  vivas no final */
    isum = 0;
    for(i=1; i<=live_ni; i++){
        for(j=1; j<=live_nj; j++){
            isum = isum + new_arr[i][j];
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
        //delete old_arr[i];
        //delete new_arr[i];
        free(old_arr[i]);
        free(new_arr[i]);
    }

    //delete old_arr;
    //delete new_arr;
    free(old_arr);
    free(new_arr);

    MPI_Finalize();

    return 0;
}
