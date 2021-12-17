# Stack Compilation and Crosscompilation Example

[![badge](https://github.com/rustyducks/coin/actions/workflows/cpp_linux_x86.yaml/badge.svg)](https://github.com/rustyducks/coin/actions/workflows/cpp_linux_x86.yaml)

This document is intended for showing an example of a workspace setup, compilation and installation of the dependencies and compilation of the coin executable, both for native compilation and cross compilation for linux ARMhf (Raspberry Pi).

## Compiling and installing natively (done on Ubuntu 18.04)

### Setting up a workspace

First place yourself in your wanted directory (your home `cd ~` may be a good choice), and create a dedicated environment:

```bash
mkdir rustyducks_ws && cd rustyducks_ws
mkdir include  # Optional
mkdir lib  # Optional
mkdir src
```

Export this helpful environment variable, which will be used all along this example:

```bash
export RUSTYDUCKS_WS_PATH=$(pwd)
```

### Install binary dependencies

As we will want to crosscompile, the only "binary" dependency we get is Eigen3 (as its an header only library). Of course, we will also need the crosscompiling toolchain [see this section](#crosscompiling).

```bash
sudo apt update && sudo apt install libeigen3-dev
```

### <a name="cloneallnative"></a>Clone all the needed repositories

There's no time to discriminate between the "dependencies" and Rusty Ducks' codebase, we clone everything directly to src (for simplicity purposes, feel free to do otherwise).

```bash
cd $RUSTYDUCKS_WS_PATH/src

# ducklink_cpp, /!\ contains submodules
git clone git@github.com:rustyducks/ducklink_cpp.git
cd ducklink_cpp
git submodule update --init --recursive
cd ..  # Important

# geometry_tools
git clone git@github.com:rustyducks/geometry_tools.git

# navigation
git clone git@github.com:rustyducks/navigation.git

# pygargue/anatidae
git clone git@github.com:rustyducks/pygargue.git

# coin
git clone git@github.com:rustyducks/coin.git

# Google Protocol Buffer (at the end, as it can be quite long) /!\ contains submodules
git clone --branch v3.1.0 git@github.com:protocolbuffers/protobuf.git
cd protobuf
git submodule update --init --recursive
cd ..
```

### Compiling and installing

#### Protobuf

```bash
cd $RUSTYDUCKS_WS_PATH/src/protobuf
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH -DCMAKE_BUILD_TYPE=Release -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_WITH_ZLIB=OFF ../cmake/
make -j8
make install
```

#### ducklink_cpp

```bash
cd $RUSTYDUCKS_WS_PATH/src/ducklink_cpp
mkdir build && cd build 
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH -DCMAKE_BUILD_TYPE=Release ..
make -j8
make install
```

#### geometry_tools

```bash
cd $RUSTYDUCKS_WS_PATH/src/geometry_tools
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH -DCMAKE_BUILD_TYPE=Release ..
make -j8
make install
```

#### navigation

```bash
cd $RUSTYDUCKS_WS_PATH/src/navigation
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH -DCMAKE_BUILD_TYPE=Release ..
make -j8
make install
```

#### coin (at last)

```bash
cd $RUSTYDUCKS_WS_PATH/src/coin
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH -DCMAKE_BUILD_TYPE=Release ..
make -j8
make install
```

### Run it!

```bash
cd $RUSTYDUCKS_WS_PATH/src/coin/build
./Coin
```

## <a name="crosscompiling"></a>Crosscompiling for Linux ARM (Raspberry Pi)

### Environment setup

Create a new environment for everything built for ARM. A solution can be to create it as a subfolder of the native one, avoiding to duplicate source code while keeping everything (kind of) nice and tidy:

```bash
# With the variable RUSTYDUCKS_WS_PATH pointing to the native workspace
cd $RUSTYDUCKS_WS_PATH
export RUSTYDUCKS_WS_PATH_ARM=$RUSTYDUCKS_WS_PATH/armlinux
mkdir $RUSTYDUCKS_WS_PATH_ARM
```

### Toolchain installation

```bash
sudo apt update && sudo apt install g++-arm-linux-gnueabihf gcc-arm-linux-gnueabihf
```

### Clone all the needed repositories

*(Optional if already done for native)* Same as the native compilation, [see here](#cloneallnative).

### Compiling and installing for ARM

The biggest change from native compiling is for protobuf, where a cmake toolchain has to be manually set. For the other repositories, they come with a convenient cmake option `CROSSCOMPILE_ARM` setting the toolchain (the tools path can still be configured via the environment variable `LINUX_ARM_TOOLCHAIN_PATH`, but defaults to the installation directory of the binaries installed with apt).

#### Protobuf for ARM

```bash
cd $RUSTYDUCKS_WS_PATH/src/protobuf
mkdir buildarm && cd buildarm
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH_ARM -DCMAKE_BUILD_TYPE=Release -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_WITH_ZLIB=OFF -DCMAKE_TOOLCHAIN_FILE=$RUSTYDUCKS_WS_PATH/src/coin/cmake/arm_build.cmake ../cmake/
make -j8
make install
# Small hack, but probably optional
rm $RUSTYDUCKS_WS_PATH_ARM/bin/protoc
ln -s $RUSTYDUCKS_WS_PATH/bin/protoc $RUSTYDUCKS_WS_PATH_ARM/bin/protoc
```

#### ducklink_cpp for ARM

```bash
cd $RUSTYDUCKS_WS_PATH/src/ducklink_cpp
mkdir buildarm && cd buildarm
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH_ARM -DCROSSCOMPILE_ARM=ON ..
make -j8
make install
```

#### geometry_tools for ARM

```bash
cd $RUSTYDUCKS_WS_PATH/src/geometry_tools
mkdir buildarm && cd buildarm
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH_ARM -DCROSSCOMPILE_ARM=ON ..
make -j8
make install
```

#### navigation for ARM

```bash
cd $RUSTYDUCKS_WS_PATH/src/navigation
mkdir buildarm && cd buildarm
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH_ARM -DCROSSCOMPILE_ARM=ON ..
make -j8
make install
```

#### coin for ARM

```bash
cd $RUSTYDUCKS_WS_PATH/src/coin
mkdir buildarm && cd buildarm
cmake -DCMAKE_INSTALL_PREFIX=$RUSTYDUCKS_WS_PATH_ARM -DCROSSCOMPILE_ARM=ON ..
make -j8
```

### Run it (but not on your host...)

Copy the `Coin` executable on the target machine, and you should be able to run it!