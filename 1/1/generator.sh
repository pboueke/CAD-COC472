#!/bin/bash
SIZES=(10 50 100 500 1000 5000 10000 50000 100000 500000 1000000 5000000 10000000 15000000 20000000 25000000 30000000 35000000 40000000 45000000 50000000 55000000 60000000 65000000 70000000 75000000 80000000 85000000 90000000 95000000 100000000)

for N in "${SIZES[@]}"
do
	gcc -O stream.c -DSTREAM_ARRAY_SIZE=$N -o stream_$N
done

for N in "${SIZES[@]}"
do 
	echo "running N=$N\n" >> output.txt
	./stream_$N >> output.txt
	echo "<==================//==================>\n" >> output.txt
done

