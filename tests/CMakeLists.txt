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

file(GLOB BUSTUB_TEST_SOURCES "${PROJECT_SOURCE_DIR}/tests/*/*test.cc")

foreach (bustub_test_source ${BUSTUB_TEST_SOURCES})
  get_filename_component(bustub_test_filename ${bustub_test_source} NAME)
  string(REPLACE ".cc" "" bustub_test_name ${bustub_test_filename})
  
  add_executable(
    ${bustub_test_name}
    ${bustub_test_source}
  )

  target_link_libraries(${bustub_test_name}
    PRIVATE
      quas
      GTest::gtest_main
  )

  gtest_discover_tests(${bustub_test_name})
endforeach (bustub_test_source ${BUSTUB_TEST_SOURCES})
