add_library(common error.h result.h config.h shared.h shared.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:common>
    PARENT_SCOPE)
