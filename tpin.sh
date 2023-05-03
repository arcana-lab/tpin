#!/bin/bash

tpin_dir="$(dirname "$0")"

nthreads_per_core=$(lscpu | grep "Thread(s) per core" | egrep -o "[0-9]+")
nsockets=$(lscpu | grep "Socket(s)" | egrep -o "[0-9]+")

$tpin_dir/tpin.out $nthreads_per_core $nsockets
