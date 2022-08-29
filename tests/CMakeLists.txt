cmake_minimum_required(VERSION 3.14)

# GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
  hello_test
  hello_test.cc
)

target_link_libraries(hello_test
  PRIVATE
    quas
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)
