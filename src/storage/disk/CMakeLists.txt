add_library(disk disk_manager.h disk_manager.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:table>
    PARENT_SCOPE)
