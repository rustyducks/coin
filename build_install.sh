#!/bin/bash

BUILD_TYPE=Debug

set -e

cd ../../ && export RUSTYDUCKS_WS_PATH=$(pwd) && echo $RUSTYDUCKS_WS_PATH
cd $RUSTYDUCKS_WS_PATH/src/protobuf
mkdir -p build
cd build && rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH -DCMAKE_BUILD_TYPE=Release -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_WITH_ZLIB=OFF ../cmake/
make -j8
make install

cd $RUSTYDUCKS_WS_PATH/src/ducklink_cpp
mkdir -p build && cd build && rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
make -j8
make install

cd $RUSTYDUCKS_WS_PATH/src/geometry_tools
mkdir -p build && cd build && rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
make -j8
make install

cd $RUSTYDUCKS_WS_PATH/src/navigation
mkdir -p build && cd build && rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
make -j8
make install

cd $RUSTYDUCKS_WS_PATH/src/coin
mkdir -p build && cd build && rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
make -j8
make install
