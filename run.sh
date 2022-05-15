#!/bin/bash
set -e

gcc -Wall Main.c -o Main.o -lm


mkdir data

L=8
for T in 0.001 0.01 0.1 0.5 1 1.5 2.0 2.269 2.5 3.0 5.0 10 100
do
	echo "T=$Z\t L=$L" 
	./Main.o -L $L -T $T -o "data/T$T L$L.csv"
done