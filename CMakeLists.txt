cmake_minimum_required (VERSION 3.12)
project (TonUINO-TNG)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_subdirectory (test)

# Hardware-independent tests for portable parent controls, radio timeout and RGB pulses.
add_executable(musicbox_policy_tests tests/musicbox/policy_test.cpp)
target_include_directories(musicbox_policy_tests PRIVATE src)
add_test(NAME musicbox_policy COMMAND musicbox_policy_tests)
