#!/bin/bash

if [[ -f "./build/FourInARowCPP" ]]; then
	./build/FourInARowCPP
else
	echo Run ./setup.bash before this script!
fi
