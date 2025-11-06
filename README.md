# utils

A collection of **common utilities and data structures** used across several related projects.

## Overview

This project provides reusable components such as data structures and helper
 utilities that are shared among the following projects:

- [ep_engine](https://github.com/wjnlim/ep_engine.git)
- [msg_pass](https://github.com/wjnlim/msg_pass.git)
- [mrwsi](https://github.com/wjnlim/mrwsi.git)

Although this is originally developed for the above projects, **`utils`** can also be used as a standalone **data structure library** for other C projects.

---

## Notes
- This library depends on pthread, so you must link with **-lpthread**
- This project is mainly for personal use, not for production codes.\
  Thus, the code may lack thorough testing, so please use it with caution.
- This library provides the following main data structures:
    - blocking_queue
    - deque
    - hash_table
    - red-black tree
---

## Build and Installation

Follow these steps to build and install the library:

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

## Usage Example
Refer to the demo program in the repository for example usage.
* [blocking_queue_demo.c](tests/blocking_queue_demo.c)
* [deque_demo.c](tests/deque_demo.c)
* [hash_table_demo.c](tests/hash_table_demo.c)
* [rb_tree_demo.c](tests/rb_tree_demo.c)

To compile your program using this library (for example, if you include #include "utils_ds/deque.h"):
```bash
gcc your_prog.c -o your_prog -I <your install directory>/include \
  <your install directory>/lib/libutils_ds.a -lpthread
```