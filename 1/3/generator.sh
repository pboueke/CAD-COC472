#!/bin/bash

SIZES=(100 200 300 400 500)
FLAGS=(O O2 O3)

for flag in "${FLAGS[@]}"
do
    g++ -${flag} matrixIJ.cpp -std=c++11 -o matrixIJ_${flag}
    g++ -${flag} matrixJI.cpp -std=c++11 -o matrixJI_${flag}
done

for flag in "${FLAGS[@]}"
do
    for size in "${SIZES[@]}"
    do
        ./matrixIJ_${flag} ${size} >> IJ_${flag}.dat
        ./matrixJI_${flag} ${size} >> JI_${flag}.dat
    done
done
