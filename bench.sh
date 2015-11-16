#!/bin/bash -e
export LD_LIBRARY_PATH='/opt/gcc-5.2.0-fdo/lib64'
cd "$(dirname "$0")"
benchmark='./build/release/benchmark'
#for name in classes hierarchy calls objc; do
for name in hierarchy; do
    for iter in {0..0}; do
        echo "Benchmark '${name}', iteration ${iter}"
        dest="./result/${name}_${iter}.csv"
        rm -f "${dest}"
        touch "${dest}"
        taskset 0x0000001 ${benchmark} ${name} >> "${dest}"
    done
done
