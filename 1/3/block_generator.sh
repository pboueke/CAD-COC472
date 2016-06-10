#!/bin/bash

BLOCKSIZES=(1000 2000 3000 4000 5000 6000 7000 8000)

cd bin
g++ -O3 ../matrixBL.cpp -std=c++11 -o matrixBL
cd ..

cd time_data
for size in "${BLOCKSIZES[@]}"
do
(sudo nice --10 ../bin/matrixBL ${size} ; ) >> block.dat
done
cd ..

