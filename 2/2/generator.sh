#!/bin/bash

RUN_PATH="/home/jawa/local/tau/x86_64/bin"

SCAL=(dynamic guided static)
SYNC=(exp)
LOOP=(collapse nocollapse)

export TAU_MAKEFILE=${HOME}/local/tau/x86_64/lib/Makefile.tau-papi-pdt-openmp-opari
export PATH=$PATH:${HOME}/local/tau/x86_64/bin
export TAU_OPTIONS=-optRevert

# start compiling
mkdir bin
cd bin

for scale in "${SCAL[@]}"
do
    $RUN_PATH/tau_cc.sh -openmp -O ../wave/main_scl_${scale}.cc -o wave_scl_${scale}.exe
done
for synch in "${SYNC[@]}"
do
    $RUN_PATH/tau_cc.sh -openmp -O ../wave/main_sync_${synch}.cc -o wave_sync_${synch}.exe
done
for lp in "${LOOP[@]}"
do
    $RUN_PATH/tau_cc.sh -openmp -O ../wave/main_loop_${lp}.cc -o wave_loop_${lp}.exe
done

cd ..
# end compiling

# running
TIMEFORMAT=%R #get time in seconds

mkdir data
cd bin

for program in *.exe
do
    echo Running \for ${program}...
    mkdir ../data/dir_${program}
    cd ../data/dir_${program}
    
    { time ../../bin/${program} ; } 2> ${program}.time.dat
    
    cd ../../bin
done

cd ..
# end running
