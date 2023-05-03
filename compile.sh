#!/bin/bash

TPC=$(lscpu | grep "Thread(s)" | egrep -o "[0-9]+")
ND=$(lscpu | grep "NUMA node(s)" | egrep -o "[0-9]+")

clang++ \
  -fopenmp \
  -DTHREADS_PER_CORE=$TPC \
  -DNUMA_DOMAINS=$ND \
  -o tpin \
  tpin.cpp
