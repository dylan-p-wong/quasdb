add_library(quas)

target_sources(quas
  PRIVATE
    hello.cc
  PUBLIC
    hello.h
  )

target_include_directories(quas
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

# we use this to get code coverage
# flags are only valid with the GNU compiler and on Linux
if(CMAKE_CXX_COMPILER_ID MATCHES GNU AND CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  target_compile_options(quas
    PUBLIC
      "--coverage"
    )
  target_link_options(quas
    INTERFACE
      "--coverage"
    )
endif()
