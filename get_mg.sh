#!/bin/bash

git clone https://github.com/memgraph/mgclient libs/mgclient
cd libs/mgclient

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=libs/ ..

make
