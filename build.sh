#! /bin/bash

commonFlags="-Wall -Werror -g -O0"
commonLinkerFlags="-g"
c++ $commonFlags src/Tafl.cpp -o build/tafl $commonLinkerFlags
