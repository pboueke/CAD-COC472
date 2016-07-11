#!/bin/bash

TIMEFORMAT=%R

THREADS=(1 2 4 8)
SIZES=(20 21 22)

g++ -O3 quicksort.cpp -o qs.x -fopenmp

mkdir data
cd data

for t in "${THREADS[@]}"
do
    for s in "${SIZES[@]}"
    do
        for run in {1..20}
        do
            (time  ../qs.x $s $t) 2>> qs_t${t}_s${s}.dat
        done
    done
done

