cmake_minimum_required(VERSION 3.0)

include(ExternalProject)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR})

ExternalProject_Add(googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG main
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

include(GoogleTest)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

include_directories("../../src/tonella")

# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests tests.cpp test_player.cpp test_statemachine.cpp ../../src/tonella/player.cpp ../../src/tonella/statemachine.cpp)
target_link_libraries(runTests gtest gmock gtest_main pthread)

# Code coverage binary
add_executable(runTestsCov tests.cpp test_player.cpp test_statemachine.cpp ../../src/tonella/player.cpp ../../src/tonella/statemachine.cpp)
target_link_libraries(runTestsCov gtest gmock gtest_main pthread gcov)
target_compile_options(runTestsCov PRIVATE "-g")
target_compile_options(runTestsCov PRIVATE "-fprofile-arcs")
target_compile_options(runTestsCov PRIVATE "-ftest-coverage")

