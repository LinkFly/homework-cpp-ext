cmake_minimum_required(VERSION 3.16.2)
project(HW7_Serialization)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(
	hw7-serialization 
	persons.pb.h persons.pb.cc
	interfaces.h
	StudentsGroup.h StudentsGroup.cpp
	hw7-serialization.cpp
	persons.proto
)
target_link_libraries(hw7-serialization ${CONAN_LIBS})

