## Contents

This repository contains several synthetic benchmarks of exception handling routines,
such as exception allocation and throwing, stack unwinding and handler search.

It also has some test cases which can be used to profile the mentioned routines.

## Requirements

The code is written in C++14 and Objective C and requies GCC 4.9 or later to
compile it.

The benchmarks use [PAPI](http://icl.cs.utk.edu/papi/) library to read
the performance counters. It is available in major Linux distros (for example,
`libpapi-dev` package in Debian)

CMake 3.0 or later is required to build the tests.

## Usage

Run [make_all.sh](make_all.sh) to compile and [bench.sh](bench.sh) to benchmark.
The result will be output to `result` directory.

Rename [config.sh.example](config.sh.example) into `config.sh` and modify to set
custom path to GCC.

## Results

The [results](result/result.ipynb) are avaiable as IPython notebook.

## References
1. Technical Report on C++ Performance [TR18015](http://www.open-std.org/jtc1/sc22/wg21/docs/TR18015.pdf)
2. [Itanium C++ ABI: Exception Handling](https://mentorembedded.github.io/cxx-abi/abi-eh.html)
3. [System V Application Binary Interface. AMD64 Architecture Processor Supplement](http://www.x86-64.org/documentation/abi.pdf),
[v. 0.99.7](http://www.x86-64.org/documentation/abi-0.99.7.pdf)
4. [DWARF Debugging Information Format. Version 4](http://dwarfstd.org/doc/DWARF4.pdf)
5. Ian Lance Tayolor's blog posts
 - [.eh_frame](http://www.airs.com/blog/archives/460)
 - [.eh_frame_hdr](http://www.airs.com/blog/archives/462)
 - [.gcc_except_table](http://www.airs.com/blog/archives/464)
6. [Exception Handling in LLVM](http://llvm.org/docs/ExceptionHandling.html)
