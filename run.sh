#!/bin/bash
set -e

gcc -Wall Main.c -o Main.o -lm


#mkdir data

L=16

for T in 1 1.5
do
	for i in $(seq 1 10)
	do
		echo "T=$Z\t L=$L i=$i"
		./Main.o -L $L -T $T -ms 50 -o "22-05-19.csv"
	done
done

for T in 2.0 2.269 2.5 5.0 10
do
	echo "T=$Z\t L=$L i=$i"
	./Main.o -L $L -T $T -ms 500 -o "22-05-19.csv"
done