#!/bin/bash

PROC=(1 2 3 4)

#compile
mpicc mpi_hello_world.c -o hello

#run
for p in "${PROC[@]}"
do
  mpirun -np ${p} ./hello
done
