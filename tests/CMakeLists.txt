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

include(GoogleTest)

file(GLOB TEST_SOURCES "${PROJECT_SOURCE_DIR}/tests/*/*test.cc")

foreach (test_source ${TEST_SOURCES})
  get_filename_component(test_filename ${test_source} NAME)
  string(REPLACE ".cc" "" test_name ${test_filename})
  
  add_executable(
    ${test_name}
    ${test_source}
  )

  target_link_libraries(${test_name}
    PRIVATE
      quas
      GTest::gtest_main
  )

  gtest_discover_tests(${test_name})
endforeach (test_source ${TEST_SOURCES})
