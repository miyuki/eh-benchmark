#!/bin/bash -e
cd "$(dirname "$0")"

if [[ -e config.sh ]]; then
    . config.sh
fi

rm -rf './build'
mkdir -p './build/release'
cd './build/release'
if [[ -n ${GCC_BIN} ]]; then
    cmake '../..' -DCMAKE_BUILD_TYPE=RelWithDebInfo \
                -DCMAKE_CXX_COMPILER="${GCC_BIN}/g++" \
                -DCMAKE_C_COMPILER="${GCC_BIN}/gcc"
else
    cmake '../..' -DCMAKE_BUILD_TYPE=RelWithDebInfo
fi

make "-j$(nproc)" VERBOSE=1
