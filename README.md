# Tools
Yet another small C++ generic library, extending the standard library for ease
of use.

## Content
- DataBase: a singleton used to parse specifically formatted files and present
  the content as key/values.
  Written before I heard of YAML... so considered as deprecated.
- DualStream: a simplistic class to print to screen and to a log file at the
  same time. Used as `cout/cerr`.
- Exception: provides an `except` class derived of `std::exception` but
  accepting any number of arguments to format all of them into a string.
  See String below.
- Some useful constants (Pi and derivatives mainly) and templatized functions:
  square, cube, power with an integer exponent, sum of square of any number of
  numbers, square root of the sum of squares, and difference of the square of
  2 numbers.
- ProgressBar: a progress bar with instant and average speed.
- Rand: a class providing an integrated interfaces to all standard random
  distributions, and allowing to shoot 3-dimensional vectors.
- Regex: a class providing an integrated interfaces to regular expressions,
  built on top of the standard regex library.
- String: two functions `cat` and `scat` allowing to concatenate an arbitrary
  number of arguments of any types into a string, and a compile time hash
  function to allow to switch on strings.
- Vector3: yet another implementation of the 3-dimensional mathematical vector.

## Build
The library is built with `cmake` and has been tested with `g++` and `clang`.

A Makefile is provided to run `cmake` then `make` is a newly created directory,
with the following options:
- all: same as install_release
- debug: same as install_debug
- build_release: create a release directory and run `cmake`
- compile_release: build_release + compile the project
- install_release: build_release + compile_release + install locally the project
- build_debug: create a debug directory and run `cmake`
- compile_debug: build_debug + compile the project
- install_debug: build_debug + compile_debug + install locally the project
- clean: clean_lib_bin + remove the release directory
- clean_debug: clean_lib_bin + remove the debug directory
- clean_lib_bin: remove the install directory
