#!/bin/bash
SIZES=(10 50 100 500 1000 5000 10000 50000 100000 500000 1000000 10000000 20000000 30000000 40000000 50000000 60000000 70000000 80000000 90000000 100000000)

RUN_PATH="/home/jawa/local/hpctoolkit/bin"


for N in "${SIZES[@]}"
do
    mkdir stream-$N
    cd stream-$N
	  gcc -g -O -DSTREAM_ARRAY_SIZE=$N ../stream.c -o stream_$N
    cd ../
done

for N in "${SIZES[@]}"
do
    cd stream-$N
    $RUN_PATH/hpcrun -e PAPI_L2_TCM -e PAPI_L1_TCM -e CPUTIME ./stream_$N
    cd ../
done

for N in "${SIZES[@]}"
do
    cd stream-$N
    $RUN_PATH/hpcstruct ./stream$N
    $RUN_PATH/hpcprof -S stream$N.hpcstruct -I~ hpctoolkit-stream_$N-measurements
    cd ../
done
