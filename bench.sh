#!/bin/bash -e
cd "$(dirname "$0")"

if [[ -e config.sh ]]; then
    . ./config.sh
fi

if [[ -n "${GCC_LIB}" ]]; then
    export LD_LIBRARY_PATH="${GCC_LIB}"
fi

benchmark='./build/release/benchmark'
for name in classes hierarchy calls objc; do
    for iter in {0..0}; do
        echo "Benchmark '${name}', iteration ${iter}"
        dest="./result/${name}_${iter}.csv"
        rm -f "${dest}"
        touch "${dest}"
        taskset 0x0000001 ${benchmark} ${name} >> "${dest}"
    done
done
