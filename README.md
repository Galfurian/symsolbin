# Symsolbin
Symbolic resolution and simulation of analog circuits.

## Table of Contents

 1. [Prerequisites](#1-prerequisites)
 2. [Compiling Symsolbin](#2-compiling-symsolbin)
 3. [Compiling and executing the examples](#3-compiling-and-executing-the-examples)
 4. [Contributors](#4-contributors)

## 1. Prerequisites

Symsolbin is compatible with the main **unix-based** operating systems. It has
been tested with *Ubuntu*, *WSL1*, and *WSL2*.

For compiling the system:

 - g++
 - make
 - cmake
 - git
 - GiNaC
 - ccmake (suggested)
 
### installation Prerequisites

Under **Ubuntu**, you can type the following commands:

```bash
sudo apt-get update && sudo apt-get upgrade -y
sudo apt-get install -y g++ make cmake git libginac-dev
```

*[Back to the Table of Contents](#table-of-contents)*

## 2. Compiling Symsolbin

Under **Ubuntu**, compile with:

```bash
cd <clone_directory>
mkdir build
cd build
cmake ..
make
```

*[Back to the Table of Contents](#table-of-contents)*

## 3. Compiling and executing the examples

To execute the examples, you need to first enable their compilation from the
`build` folder, as follows:

```bash
cd build
cmake .. -DSYMSOLBIN_BUILD_EXAMPLES=ON
make
```

Then, you can execute any of the examples that are now compiled.

*[Back to the Table of Contents](#table-of-contents)*

## 4. Contributors

* [Enrico Fraccaroli](https://github.com/Galfurian)

*[Back to the Table of Contents](#table-of-contents)*
