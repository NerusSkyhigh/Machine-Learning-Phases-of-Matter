#!/bin/bash
set -e

gcc -Wall Main.c -o Main.o -lm


#mkdir data

L=16
for T in 0.01 0.1 0.5 1 1.5 2.0 2.269 2.5 3.0 5.0 10 100
do
	for i in $(seq 1 20)
	do
		echo "T=$Z\t L=$L i=$i"
		./Main.o -L $L -T $T -ms 100 -o "22-05-19.csv"
	done
done