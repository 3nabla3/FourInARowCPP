#!/bin/bash

# Setup the project with cmake
echo "Building the compilation environment"
cmake . -B build -G "Unix Makefiles"

# make sure the latest version is compiled before running
cd build || exit
make -j "$(nproc)"
cd ..

# Run the executable
echo "Running the executable"
chmod +x run.bash
./run.bash
