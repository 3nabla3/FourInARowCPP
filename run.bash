#!/bin/bash

# make sure the latest version is compiled before running

cd build || exit
make -j "$(nproc)"
cd ..

./build/FourInARowCPP
