cmake_minimum_required(VERSION 3.14)

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

message("${GeometryTools_LIBRARIES}")

find_package(NoWay)
if (NOT ${NoWay_FOUND})
  message("'NoWay' not found on local installation, retrieveing it from Github.")
  FetchContent_Declare(
    NoWay
    GIT_REPOSITORY https://github.com/rustyducks/NoWay.git
    GIT_TAG master
  )
  FetchContent_MakeAvailable(NoWay)
endif()

add_subdirectory(deps)

add_executable(Coin src/main.cpp src/Behavior/Slave.cpp src/Robot.cpp src/Communication/IvyHandler.cpp 
            src/Communication/Ducklink/SerialDucklink.cpp src/Actuator/Arm.cpp src/Actuator/Hat.cpp src/Communication/Ducklink/UDPDucklink.cpp)
target_include_directories(Coin PUBLIC include ${Serialib_INCLUDE_DIR})
target_link_libraries(Coin PUBLIC GeometryTools Navigation Serialib Protoduck)
set_target_properties(Coin PROPERTIES CXX_STANDARD 14)
target_compile_options(Coin PRIVATE -Wall -Wextra -Wpedantic -Werror)



