cmake_minimum_required(VERSION 3.16.2)
project(HW8_SerializationTest)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(
	hw7-serialization
	hw7-serialization.cpp
	persons.pb.h persons.pb.cc
	interfaces.h
	StudentsGroup.h StudentsGroup.cpp
	hw7-serialization.cpp
	persons.proto
)
target_link_libraries(hw7-serialization ${CONAN_LIBS})

add_executable(
	hw8-serialization_test
	hw8-serialization_test.cpp
	persons.pb.h persons.pb.cc
	interfaces.h
	StudentsGroup.h StudentsGroup.cpp
	persons.proto)
target_link_libraries(hw8-serialization_test ${CONAN_LIBS})
