# Tools

<!-- [![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black) -->
[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit&logoColor=white)](https://github.com/pre-commit/pre-commit)

Yet another small C++ generic library, but with an automatic Python binding!

## Content

- `BaseError`: provides a `BaseError` class derived of `std::exception` but
  accepting any number of arguments to format all of them into a string.
  See `StringUtils` below.
- `DataBase`: a singleton used to parse specifically formatted files and present
  the content as key/values.
  Written before I heard of YAML... so considered as deprecated.
- `Display`: set of function and operator to help printing, logging...
- `DualStream`: a simplistic class to print to screen and to a log file at the
  same time. Used as `cout/cerr`.
- `make_unique`: provides the function `make_unique` for C++11
- `Math`: some useful constants (Pi and derivatives mainly) and templatized functions:
  square, cube, power with an integer exponent, sum of square of any number of
  numbers, square root of the sum of squares, and difference of the square of
  2 numbers.
- `ProgressBar`: a progress bar with instant and average speed.
- `Rand`: a class providing an integrated interfaces to all standard random
  distributions, and allowing to shoot 3-dimensional vectors.
- `Regex`: a class providing an integrated interfaces to regular expressions,
  built on top of the standard regex library. Do not use for big data parsing, since
    the standard regex implementation is known to be significantly slower than several
    widely used free implementations (such as boost).
- `StringUtils`: string manipulation functions: concatenation of an arbitrary number of
  arguments of any types into a string, split, join and a compile time hash function to
  allow to switch on strings.
- `TypeErasure`: provides a wrapper for functions with arbitrary arguments
- `Vector3`: yet another implementation of the 3-dimensional mathematical vector.

## Build

### Layout

The library is built with `cmake` and has been tested with `g++` and `clang`.

The build layout is included in the repository, with the `source`, `build` and
`install` directories, the `CMakeLists.txt` file is therefore in `source/`.

The installation step is required, because some headers are generated
at the compilation step in the `build/` subdirectories, so all the headers
are only available in one place after installation.

The library contains an optional binding to Python, based on
[Cppyy](https://cppyy.readthedocs.io). The binding will be automatically generated
at the compilation if the optional prerequisites are installed and the Python
virtual environment activated (with `poetry shell`).

### Prerequisites

Required:

- `make`
- `cmake`: >= 3.2
- `gcc`: >= 5 or `clang`: >= 5

Optional:

- `clang`: >= 10
- `libclang-dev`: same as clang
- `python`: >= 3.6
- `poetry`: >= 1.1

then run `poetry install` to install the optional Python packages

### Option

A Makefile is provided at the root to run `cmake` then `make` in a single step,
with the following options:

- `all`: same as `install_release`
- `debug`: same as `install_debug`
- `build_release`: create a `release/` directory in `build/` and run `cmake`
- `compile_release`: `build_release` + compile the project
- `install_release`: `build_release` + `compile_release` + install locally the project
- `build_debug`: create a `debug` directory in `build/` and run `cmake`
- `compile_debug`: `build_debug` + compile the project
- `install_debug`: `build_debug` + `compile_debug` + install locally the project
- `clean`: `clean_lib_bin` + remove the `release` directory
- `clean_debug`: `clean_lib_bin` + remove the `debug` directory
- `clean_lib_bin`: remove the `install` directory

## Test

Once the librairy has been built, including the Python binding, run `pytest`:

    # Building
    poetry shell
    make -j

    # Actual tests
    pytest

## Contribute

### Tooling set

A set of formatters and static analysis tools is used to guarantee the homogeneity
of the code and catch mistakes:

- `yamlfmt`: YAML formatter
- `clang-format` >= 10: C++ formatter
- `black`: Python formatter
- `cpplint`: C++ static analysis tool
- `cppcheck`: C++ static analysis tool
- `flake8`: Python static analysis tool
- `pyupgrade`: Python static analysis tool

plus a bunch of other small tools from
[pre-commit-hooks](https://github.com/pre-commit/pre-commit-hooks), and of course
these tools are expected to be called through [pre-commit](https://pre-commit.com/).

The C++ tools are expected to be installed independently, while the other tools
are either installed by `pre-commit`, either installed as `dev` dependencies with
[poetry](https://python-poetry.org/). This means that committing is expected
to *always* be made in the project Python virtual environment. The best solution is
to run `poetry shell` once and for all in the repository root when entering the project.
An other solution is to use `poetry run git commit ...` (for configuration of an IDE)
for instance.

### Tooling installation

- `pre-commit`, as a Python tool, can be installed with:

    - `pipx` (preferred)

            pipx install pre-commit

    - `pip` (or `pip3` on Debian-like distribution)

            pip install -U pre-commit

    - `snap` (system-wide)

            sudo snap install pre-commit

  and perhaps from your distribution package manager.

  Once `pre-commit` is installed, activate it with `pre-commit install` at the
  repository root, and test is with `pre-commit run`.

- `poetry` is also a Python tool but according to the
  [documentation](https://python-poetry.org/docs/#installation), it is best
  installed with its own installer:

        curl -sSL \
        https://raw.githubusercontent.com/python-poetry/poetry/master/get-poetry.py \
        | python -

  or on Debian-like distribution (using `python3` instead of `python` which is
  `python2`):

        curl -sSL \
        https://raw.githubusercontent.com/python-poetry/poetry/master/get-poetry.py \
        | python3 -

  Once `poetry` is installed, run `poetry install --dev` to install all the project
  packages and the development tooling.

  If the package installation fails, try removing the default Python `virtualenv`
  package and reinstall it with `pip`. On Debian-like distribution

      sudo apt remove --purge python3-virtualenv virtualenv
      pip3 install virtualenv

- `cppcheck` can be installed from your package manager (`apt`, `yum` and `brew` at
  least) or downloaded from the [site](http://cppcheck.sourceforge.net/#download).

  On Debian-like distribution, use:

      sudo apt install cppcheck

- `clang-format` can be installed from your package manager. If not, consider
  changing/upgrading your distribution or see the
  [download page](https://releases.llvm.org/download.html).

  If the default version is too low, explicitly install a higher version and use
  `update-alternative` to link the given version as the default `clang-format`.

  On Debian-like distribution, use:

      sudo apt install clang-format-11
      sudo update-alternatives --install /usr/bin/clang-format clang-format \
        /usr/bin/clang-format-11 50

- `cpplint`, a Python tool, is either installed through `poetry`, either installed
  at a higher level with `pip`, `pipx` or a package manager.

All together, on Ubuntu 20.04 or higher, run:

    sudo apt install make cmake gcc g++ clang-11 clang-format-11 cppcheck \
        libclang-11-dev python3 python3-pip
    sudo apt remove --purge python3-virtualenv virtualenv
    pip3 install virtualenv pre-commit
    curl -sSL \
        https://raw.githubusercontent.com/python-poetry/poetry/master/get-poetry.py \
        | python3 -
    # cd path/to/repository/
    poetry install --dev
    poetry shell
    pre-commit install
    pre-commit run
