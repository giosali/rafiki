#!/bin/sh

# Creates ./build
cmake -E make_directory build

cd build

# Specifies that the CMakeLists.txt file to be used is the
# one in ../test
cmake -DCMAKE_BUILD_TYPE=Release ../test

cmake --build .
cd ..
rm -rf build
