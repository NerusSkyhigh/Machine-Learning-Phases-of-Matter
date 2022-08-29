#!/bin/bash
set -e
export LC_NUMERIC="en_US.UTF-8"

gcc -Wall Main.c -o Main.o -lm


L=16

for T in $(seq 1.0 0.1 5.0)
do
	for i in $(seq 1 10)
	do
		echo "T=$T \t L=$L i=$i"
		./Main.o -L $L -T $T -ms 50 -o "22-05-20.csv"
	done
done

for T in $(seq 5.0 1.0 10.0)
do
		echo "T=$T \t L=$L i=$i"
		./Main.o -L $L -T $T -ms 100 -o "22-05-20.csv"
done