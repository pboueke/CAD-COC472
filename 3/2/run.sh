#!/bin/bash

PROC=(2 4 8 16)

#compile
mpicc helloBsend.c -o msg

#run
for p in "${PROC[@]}"
do
  mpirun -n ${p} ./msg
done
