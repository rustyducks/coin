if(DEFINED ENV{LINUX_ARM_TOOLCHAIN_PATH})
    set (LINUX_ARM_TOOLCHAIN_PATH $ENV{LINUX_ARM_TOOLCHAIN_PATH})
else()
    set (LINUX_ARM_TOOLCHAIN_PATH /usr/lib/ccache)
endif()

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER ${LINUX_ARM_TOOLCHAIN_PATH}/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${LINUX_ARM_TOOLCHAIN_PATH}/arm-linux-gnueabihf-g++)
set(CMAKE_FIND_ROOT_PATH /usr/lib/ccache)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)