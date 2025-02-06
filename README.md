# Orz

## Overview
----------------
Orz is a Cross-platform Protable library. 
Using GNU extension syntax, it is best to compile using gcc/clang, 
Or the compiler supports GNU extensions.
Unit test cases indicate how to use the function.

## Build 
### Before build
If you want build test, Install GoogleTest

### Building
```bash
mkdir build 
cd build

# Build without test
cmake ..                    

# Build with test
cmake -DORZ_TEST=1 ..       

make
```

### After build
Run test
```bash
make test
```

If you want Install the library and headers
```bash
make install
```
