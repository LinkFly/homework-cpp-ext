cmake_minimum_required(VERSION 3.16.2)
project(LESSON8_GTEST)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#message("CMAKE_BINARY_DIR = " ${CMAKE_BINARY_DIR})
#message("CONAN_LIBS = " ${CONAN_LIBS})
conan_basic_setup()
#message("CONAN_LIBS = " ${CONAN_LIBS})

set(LIBS_DIR libs)

set(MINMAX_LIBNAME minmax)
set(MINMAX_LIB ${LIBS_DIR}/${MINMAX_LIBNAME}.cpp)
set(MINMAX_HEADER ${LIBS_DIR}/${MINMAX_LIBNAME}.h)

include_directories(${LIBS_DIR})
add_library(MINMAX_LIBNAME STATIC ${MINMAX_LIB} ${MINMAX_HEADER})

add_executable(lesson8-gtest lesson8-gtest.cpp)
target_link_libraries(lesson8-gtest ${CONAN_LIBS} MINMAX_LIBNAME)

#set(MYTESTVAR "somelibs/path/")
#message("MYTESTVAR: " ${MYTESTVAR})
#unset(MYTESTVAR)
#message("MYTESTVAR: " ${MYTESTVAR})
