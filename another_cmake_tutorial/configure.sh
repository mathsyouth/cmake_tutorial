#! /bin/sh

cmake -S . -B out/build/ -DUSE_ADDER=ON -DCMAKE_INSTALL_PREFIX="./out"
# cmake -S . -B out/build/ -DUSE_ADDER=OFF
