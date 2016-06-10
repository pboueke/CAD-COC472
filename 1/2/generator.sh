#!/bin/bash
SIZES=(20000000 30000000 40000000 50000000 60000000 70000000 80000000 90000000 10000000)

RUN_PATH="/home/jawa/local/hpctoolkit/bin"


for N in "${SIZES[@]}"
do
    mkdir stream-$N
    cd stream-$N
	  gcc -g -O -DSTREAM_ARRAY_SIZE=$N ../../stream.c -o stream$N
    cd ../
done

for N in "${SIZES[@]}"
do
    cd stream-$N
    $RUN_PATH/hpcrun -e PAPI_L2_TCM -e PAPI_L1_TCM -e CPUTIME ./stream$N
    cd ../
done

for N in "${SIZES[@]}"
do
    cd stream-$N
    $RUN_PATH/hpcstruct ./stream$N
    $RUN_PATH/hpcprof -S stream$N.hpcstruct -I~ hpctoolkit-stream$N-measurements
    cd ../
done
