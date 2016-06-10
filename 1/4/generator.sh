#!/bin/bash

RUN_PATH="/home/jawa/local/hpctoolkit/bin"

mkdir data
cd data

$RUN_PATH/hpcrun -e PAPI_L2_TCM -e PAPI_L1_TCM -e CPUTIME ../wave/wave.exe
$RUN_PATH/hpcstruct ../wave/wave.exe
$RUN_PATH/hpcprof -S wave.exe.hpcstruct -I~ hpctoolkit-wave.exe-measurements

