cmake_minimum_required(VERSION 3.14)

project(coin VERSION 0.1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
  GeometryTools
  GIT_REPOSITORY https://github.com/rustyducks/geometry_tools.git
  GIT_TAG main
)
FetchContent_MakeAvailable(GeometryTools)


FetchContent_Declare(
  Navigation
  GIT_REPOSITORY https://github.com/rustyducks/navigation.git
  GIT_TAG main
)
FetchContent_MakeAvailable(Navigation)


FetchContent_Declare(
  NoWay
  GIT_REPOSITORY https://github.com/rustyducks/NoWay.git
  GIT_TAG master
)
FetchContent_MakeAvailable(NoWay)

add_subdirectory(deps)

add_executable(Coin src/main.cpp src/Behavior/Slave.cpp src/Robot.cpp src/Communication/IvyHandler.cpp 
            src/Communication/Ducklink/SerialDucklink.cpp src/Actuator/Arm.cpp)
target_include_directories(Coin PUBLIC include ${Serialib_INCLUDE_DIR})
target_link_libraries(Coin PUBLIC GeometryTools Navigation NoWay Serialib Protoduck)
set_target_properties(Coin PROPERTIES CXX_STANDARD 14)
target_compile_options(Coin PRIVATE -Wall -Wextra -Wpedantic -Werror)



