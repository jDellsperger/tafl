#! /bin/bash

commonFlags="-Wall -Werror -g -O0"
commonLinkerFlags="-g"
c++ $commonFlags tafl.cpp -o tafl -ldl $commonLinkerFlags

