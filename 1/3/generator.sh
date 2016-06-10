#!/bin/bash

SIZES=(10 20 30 40)
FLAGS=(O O2 O3)

mkdir bin
for flag in "${FLAGS[@]}"
do
    cd bin
    g++ -${flag} ../matrixIJ.cpp -std=c++11 -o matrixIJ_${flag}
    g++ -${flag} ../matrixJI.cpp -std=c++11 -o matrixJI_${flag}
    cd ..
done

mkdir time_data
cd time_data
for flag in "${FLAGS[@]}"
do
    for size in "${SIZES[@]}"
    do
        ../bin/matrixIJ_${flag} ${size} >> IJ_${flag}.dat
        ../bin/matrixJI_${flag} ${size} >> JI_${flag}.dat
    done
done
cd ..

RUN_PATH="/home/jawa/local/hpctoolkit/bin"

for flag in "${FLAGS[@]}"
do
    mkdir hpc-${flag}
    cd hpc-${flag}
    
    $RUN_PATH/hpcrun -e PAPI_L2_TCM -e PAPI_L1_TCM -e CPUTIME ../bin/matrixIJ_${flag}
    $RUN_PATH/hpcstruct ../bin/matrixIJ_${flag}
    $RUN_PATH/hpcprof -S matrixIJ_${flag}.hpcstruct -I~ hpctoolkit-matrixIJ_${flag}-measurements
    
    $RUN_PATH/hpcrun -e PAPI_L2_TCM -e PAPI_L1_TCM -e CPUTIME ../bin/matrixJI_${flag}
    $RUN_PATH/hpcstruct ../bin/matrixJI_${flag}
    $RUN_PATH/hpcprof -S matrixJI_${flag}.hpcstruct -I~ hpctoolkit-matrixJI_${flag}-measurements
    
    cd ..
done
