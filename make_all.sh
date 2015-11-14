#!/bin/bash -e
cd "$(dirname "$0")"
rm -rf './build'
mkdir -p './build/release'
cd './build/release'
cmake '../..' -DCMAKE_BUILD_TYPE=Release \
              -DCMAKE_CXX_COMPILER='/opt/gcc-5.2.0-fdo/bin/g++' \
              -DCMAKE_C_COMPILER='/opt/gcc-5.2.0-fdo/bin/gcc'
make "-j$(nproc)" VERBOSE=1
