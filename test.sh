#!/bin/sh

# Creates ./build
cmake -E make_directory build

cd build

# Specifies that the CMakeLists.txt file to be used is the
# one in ../test
cmake -DCMAKE_PREFIX_PATH="~/Qt/6.6.1/gcc_64/lib/cmake" -DCMAKE_BUILD_TYPE=Release ../test

cmake --build .
cd ..
rm -rf build
