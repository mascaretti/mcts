#!/bin/bash

echo -e "testing serial OXO\n"
./serial_oxo

for n_threads in {1..4}
  do
    echo -e "testing OXO with ${n_threads} thread(s) \n"
    mpiexec -np ${n_threads} speedup_oxo
done

echo -e "testing serial NIM\n"
./serial_nim

for n_threads in {1..4}
  do
    echo -e "testing NIM with ${n_threads} thread(s) \n"
    mpiexec -np ${n_threads} speedup_nim
done
