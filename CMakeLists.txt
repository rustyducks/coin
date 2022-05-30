cmake_minimum_required(VERSION 3.14)

option(CROSSCOMPILE_ARM "Cross compiling for arm architecture" OFF)


if(CROSSCOMPILE_ARM)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm_build.cmake)
endif()

project(coin VERSION 0.1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

find_package(GeometryTools)
if (NOT ${GeometryTools_FOUND})
  message("'GeometryTools' not found on local installation, retrieveing it from Github.")
  FetchContent_Declare(
    GeometryTools
    GIT_REPOSITORY https://github.com/rustyducks/geometry_tools.git
    GIT_TAG main
  )
  FetchContent_MakeAvailable(GeometryTools)
endif()

find_package(Navigation)
if (NOT ${Navigation_FOUND})
  message("'Navigation' not found on local installation, retrieveing it from Github.")
  FetchContent_Declare(
    Navigation
    GIT_REPOSITORY https://github.com/rustyducks/navigation.git
    GIT_TAG main
  )
  FetchContent_MakeAvailable(Navigation)
endif()

# find_package(NoWay)
# if (NOT ${NoWay_FOUND})
#   message("'NoWay' not found on local installation, retrieveing it from Github.")
#   FetchContent_Declare(
#     NoWay
#     GIT_REPOSITORY https://github.com/rustyducks/NoWay.git
#     GIT_TAG master
#   )
#   FetchContent_MakeAvailable(NoWay)
# endif()

find_package(Ducklink)
find_package(Protoduck)
if (NOT ${Ducklink_FOUND})
  message("'Ducklink' not found on local installation, retrieveing it from Github.")
  FetchContent_Declare(
    Ducklink
    GIT_REPOSITORY https://github.com/rustyducks/ducklink_cpp.git
    GIT_TAG master
  )
  FetchContent_MakeAvailable(Ducklink)
endif()

file(GLOB_RECURSE CoinDalek_SRC CONFIGURE_DEPENDS "src/*.cpp" EXC)
list(FILTER CoinDalek_SRC EXCLUDE REGEX ".*src/mainCrolonome\.cpp$")
list(FILTER CoinDalek_SRC EXCLUDE REGEX ".*src/mainCoupeOff\.cpp$")


file(GLOB_RECURSE CoinCrolonome_SRC CONFIGURE_DEPENDS "src/*.cpp" EXC)
list(FILTER CoinCrolonome_SRC EXCLUDE REGEX ".*src/mainDalek\.cpp$")
list(FILTER CoinCrolonome_SRC EXCLUDE REGEX ".*src/mainCoupeOff\.cpp$")

file(GLOB_RECURSE CoinCoupeOff_SRC CONFIGURE_DEPENDS "src/*.cpp" EXC)
list(FILTER CoinCoupeOff_SRC EXCLUDE REGEX ".*src/mainCrolonome\.cpp$")
list(FILTER CoinCoupeOff_SRC EXCLUDE REGEX ".*src/mainDalek\.cpp$")


add_executable(CoinDalek ${CoinDalek_SRC})
target_include_directories(CoinDalek PUBLIC include)
target_link_libraries(CoinDalek PUBLIC GeometryTools Navigation Ducklink Protoduck)
set_target_properties(CoinDalek PROPERTIES CXX_STANDARD 14)
target_compile_options(CoinDalek PRIVATE -Wall -Wextra -Wpedantic -Werror)

add_executable(CoinCrolonome ${CoinCrolonome_SRC})
target_include_directories(CoinCrolonome PUBLIC include)
target_link_libraries(CoinCrolonome PUBLIC GeometryTools Navigation Ducklink Protoduck)
set_target_properties(CoinCrolonome PROPERTIES CXX_STANDARD 14)
target_compile_options(CoinCrolonome PRIVATE -Wall -Wextra -Wpedantic -Werror)

add_executable(CoinCoupeOff ${CoinCoupeOff_SRC})
target_include_directories(CoinCoupeOff PUBLIC include)
target_link_libraries(CoinCoupeOff PUBLIC GeometryTools Navigation Ducklink Protoduck)
set_target_properties(CoinCoupeOff PROPERTIES CXX_STANDARD 14)
target_compile_options(CoinCoupeOff PRIVATE -Wall -Wextra -Wpedantic -Werror)



