# utils — Common Utilities and Data Structures

A small collection of helper utilities and data structures used across several related projects.

## Overview

`utils` provides reusable components that are shared among the following projects:

- [ep_engine](https://github.com/wjnlim/ep_engine)
- [msg_pass](https://github.com/wjnlim/msg_pass)
- [direct-access-mapreduce](https://github.com/wjnlim/direct-access-mapreduce)

This repo mainly contains small data structures and helper utilities that support the implementation of those systems.

Although primarily developed for internal use in the projects listed above,
the data structures are provided as a small library and can also be used
in other C applications.

## Provided Components

The library currently includes several basic data structures:

- blocking queue  
- deque  
- hash table  
- red-black tree  

## Notes

This project does **not aim to provide production-level data structure implementations**, and testing and edge-case coverage are intentionally minimal.

Dependencies:

- `pthread`
## Build and Installation

```bash
# 1. Clone the repository
git clone https://github.com/wjnlim/utils.git

# 2. Create a build directory
mkdir utils/build
cd utils/build

# 3. Configure with CMake
cmake -DCMAKE_INSTALL_PREFIX=<your install directory> ..

# 4. Build the data structure library
cmake --build . --target utils_ds

# 5. Install the library
cmake --install .
```

## Example Programs
Example programs are available in the repository:
* [tests/blocking_queue_demo.c](tests/blocking_queue_demo.c)
* [tests/deque_demo.c](tests/deque_demo.c)
* [tests/hash_table_demo.c](tests/hash_table_demo.c)
* [tests/rb_tree_demo.c](tests/rb_tree_demo.c)

## Using the Library
To compile a program that uses this library (for example, including utils_ds/deque.h):
```bash
gcc your_prog.c -o your_prog -I <your install directory>/include \
  <your install directory>/lib/libutils_ds.a -lpthread
```