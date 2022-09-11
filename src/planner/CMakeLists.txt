add_subdirectory(plans)

add_library(planner planner.h planner.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:planner>
    PARENT_SCOPE)
