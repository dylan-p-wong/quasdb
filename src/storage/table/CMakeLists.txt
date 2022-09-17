add_library(table rid.h tuple.h tuple.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:table>
    PARENT_SCOPE)
