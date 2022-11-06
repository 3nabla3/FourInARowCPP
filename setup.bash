#!/bin/bash

# Setup the project with cmake
echo "Building the compilation environment"
cmake . -B build -G "Unix Makefiles"
cd build || exit

# compile the project with make
if [[ $(make -j "$(nproc)") -ne 0 ]]; then
  echo "Compilation failed"
  exit
fi
cd .. || exit

# Run the executable
echo "Running the executable"

chmod +x run.bash
./run.bash



