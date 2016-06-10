FLAGS=(O3)
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