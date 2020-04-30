cmake_minimum_required(VERSION 3.0)

include(ExternalProject)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR})

ExternalProject_Add(googletest
    GIT_REPOSITORY https://github.com/google/googletest
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

include(GoogleTest)

# Locate GTest
#set(GTEST_ROOT "/home/goering/.local" CACHE PATH "Path to googletest")
#find_package(GTest REQUIRED)
#if(NOT GTEST_LIBRARY)
#   message("not found")
#endif()
#0include_directories(${GTEST_INCLUDE_DIRS})

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

#add_subdirectory(${EXTERNAL_INSTALL_LOCATION}/googletest)

#set_target_properties(gtest PROPERTIES FOLDER extern)
#set_target_properties(gtest_main PROPERTIES FOLDER extern)
#set_target_properties(gmock PROPERTIES FOLDER extern)
#set_target_properties(gmock_main PROPERTIES FOLDER extern)

#include_directories("../../../../.local/include")
include_directories("../../src/tonella")

# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests tests.cpp)
target_link_libraries(runTests gtest gmock gtest_main pthread)
