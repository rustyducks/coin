include(FindProtobuf)
find_package(Protobuf REQUIRED)
file(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/protoduck PROTOMODEL_PATH)
file(TO_NATIVE_PATH ${CMAKE_SOURCE_DIR}/include/generated PROTOBINDING_PATH)
file(MAKE_DIRECTORY ${PROTOBINDING_PATH})

file(GLOB DATAMODEL_PROTOS "${CMAKE_CURRENT_SOURCE_DIR}/protoduck/*.proto")

foreach(proto ${DATAMODEL_PROTOS})
   file(TO_NATIVE_PATH ${proto} proto_native)
   execute_process(COMMAND protoc --proto_path=${PROTOMODEL_PATH} --cpp_out=${PROTOBINDING_PATH} ${proto_native}
   RESULT_VARIABLE rv)
   # Optional, but that can show the user if something have gone wrong with the proto generation 
   if(${rv})
        message("Generation of data model returned ${rv} for proto ${proto_native}")
   endif()
endforeach(proto)
aux_source_directory(${PROTOBINDING_PATH} Protoduck_SRC_FILES)
add_library(Protoduck ${Protoduck_SRC_FILES})
target_include_directories(Protoduck PUBLIC ${Protobuf_INCLUDE_DIRS})
target_link_libraries(Protoduck ${Protobuf_LIBRARIES})

add_library(Serialib serialib/lib/serialib.cpp)
target_include_directories(Serialib PUBLIC serialib/lib)
set(Serialib_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/serialib/lib" PARENT_SCOPE)