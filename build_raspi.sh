#!/bin/bash

BUILD_TYPE=Debug

set -e

cd ../../ && export RUSTYDUCKS_WS_PATH=$(pwd) && echo $RUSTYDUCKS_WS_PATH
export RUSTYDUCKS_WS_PATH_ARM=$RUSTYDUCKS_WS_PATH/armlinux

cd $RUSTYDUCKS_WS_PATH/src/protobuf
mkdir -p buildarm
cd buildarm && rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH_ARM -DCMAKE_BUILD_TYPE=$BUILD_TYPE -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_WITH_ZLIB=OFF -DCMAKE_TOOLCHAIN_FILE=$RUSTYDUCKS_WS_PATH/src/coin/cmake/arm_build.cmake ../cmake/
make -j8
make install
rm $RUSTYDUCKS_WS_PATH_ARM/bin/protoc
ln -s $RUSTYDUCKS_WS_PATH/bin/protoc $RUSTYDUCKS_WS_PATH_ARM/bin/protoc

cd $RUSTYDUCKS_WS_PATH/src/ducklink_cpp
mkdir -p buildarm && cd buildarm && rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH_ARM -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCROSSCOMPILE_ARM=ON ..
make -j8
make install

cd $RUSTYDUCKS_WS_PATH/src/geometry_tools
mkdir -p buildarm && cd buildarm && rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH_ARM -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCROSSCOMPILE_ARM=ON ..
make -j8
make install

cd $RUSTYDUCKS_WS_PATH/src/navigation
mkdir -p buildarm && cd buildarm && rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH_ARM -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCROSSCOMPILE_ARM=ON ..
make -j8
make install

cd $RUSTYDUCKS_WS_PATH/src/coin
mkdir -p buildarm && cd buildarm && rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH_ARM -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCROSSCOMPILE_ARM=ON ..
make -j8
#make install

scp Coin dalek:~/
