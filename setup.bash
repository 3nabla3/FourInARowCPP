#!/bin/bash

# Setup the project with cmake
echo "Building the compilation environment"
cmake . -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release

# make sure the latest version is compiled before running
cd build || exit
ninja
cd ..

# Run the executable
echo "Running the executable"
chmod +x run.bash
./run.bash
