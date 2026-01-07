# Algorithms

A C++20 project implementing algorithms with GoogleTest for testing.

## Requirements

- CMake 3.20 or higher
- C++ compiler with C++20 support (GCC 10+, Clang 10+, MSVC 2019+)
- Git (for downloading GoogleTest)

## Building the Project

### Local Build

```bash
cd algorithms
mkdir -p build && cd build
cmake ..
make
```

### Alternative Generators

```bash
# Using Ninja (faster)
cmake -G Ninja ..
ninja

# Using Visual Studio (Windows)
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
```

## Running Tests

### Via CTest

```bash
cd build
ctest --output-on-failure -V
```

### Directly via Executable

```bash
cd build
./algorithms
./algorithms --gtest_filter=TestName.*
```

## Docker

```bash
docker build -t algorithms .
docker run --rm algorithms
```

## Quick Start

```bash
cd algorithms && mkdir -p build && cd build && \
cmake .. && make && ctest --output-on-failure
```
