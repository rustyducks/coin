# coin

C.O.I.N. probably stands for "Canards Oxydés Intelligence et Navigation".

## Installation

### Install dependencies

- **[geometry_tools](https://github.com/rustyducks/geometry_tools)**
- **[navigation](https://github.com/rustyducks/navigation)**
- **[protoduck](https://github.com/rustyducks/protoduck)**
- **[ducklink_cpp](https://github.com/rustyducks/ducklink_cpp)**

The dependencies may not need to be compiled and installed manually, as this project uses cmake FetchContent to retrieve and install them. However, for crosscompilation, they need to be compiled for the host and installed.

### Build and run

```bash
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=</path/to/your/workspace> ..
make -j4
./Coin
```

### Crosscompiling for Linux ARM (Raspberry Pi)

Install toolchain:

```bash
sudo apt update && sudo apt install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf
```

Compile coin:

```bash
mkdir buildarm && cd buildarm
export LINUX_ARM_TOOLCHAIN_PATH=</path/to/toolchain>  # optional: defaults to /usr/lib/ccache
cmake -DCROSSCOMPILE_ARM=ON -DCMAKE_INSTALL_PREFIX=</path/to/your/workspace/armlinux> ..  # your workspace must contain the dependencies, manually built for the host
make -j4
```