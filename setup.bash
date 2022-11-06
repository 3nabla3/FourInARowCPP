#!/bin/bash

# Setup the project with cmake
echo "Building the compilation environment"
cmake . -B build -G "Unix Makefiles"

# Run the executable
echo "Running the executable"
chmod +x run.bash
./run.bash



