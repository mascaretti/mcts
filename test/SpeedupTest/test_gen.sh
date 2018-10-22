#!/bin/bash

for n_threads in {1..4}
  do
    echo -e "testing OXO with ${n_threads} thread(s) \n"
    mpiexec -np ${n_threads} speedup_oxo
done

for n_threads in {1..4}
  do
    echo -e "testing NIM with ${n_threads} thread(s) \n"
    mpiexec -np ${n_threads} speedup_nim
done
