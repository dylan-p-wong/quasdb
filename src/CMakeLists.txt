cmake_minimum_required(VERSION 3.14)

add_subdirectory(parser)

add_library(quas STATIC ${ALL_OBJECT_FILES})

target_include_directories(quas
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

target_link_libraries(quas parser)
