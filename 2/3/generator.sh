#!/bin/bash

TIMEFORMAT=%R

THREADS=(1 2 4 8)
SIZES=(1000000 10000000 100000000 1000000000)

g++ -O3 mc_openmp.cpp -o pi.x -fopenmp

mkdir data
cd data

for t in "${THREADS[@]}"
do
    for s in "${SIZES[@]}"
    do
        for run in {1..10}
        do
            (time  ../pi.x $s $t >> mcpi_t${t}_s${s}.res.dat) 2>> mcpi_t${t}_s${s}.time.dat
        done
    done
done

