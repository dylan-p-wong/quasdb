cmake_minimum_required(VERSION 3.14)

add_subdirectory(common)
add_subdirectory(parser)
add_subdirectory(planner)
add_subdirectory(executor)
add_subdirectory(storage)

add_library(quas STATIC ${ALL_OBJECT_FILES})

target_include_directories(quas
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

target_link_libraries(quas common parser planner executor catalog page)
