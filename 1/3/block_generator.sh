#!/bin/bash

BLOCKSIZES=(4 8 16)

cd bin
g++ -O3 ../matrixBL.cpp -std=c++11 -o matrixBL
cd ..

cd time_data
for size in "${BLOCKSIZES[@]}"
do
    (sudo nice --10 ../bin/matrixBL ${size} ; ) >> block.dat
done
cd ..

