add_library(buffer buffer_manager.h buffer_manager.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:buffer>
    PARENT_SCOPE)
