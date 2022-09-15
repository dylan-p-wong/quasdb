add_library(page page.h page.cc directory_page.h table_page.h)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:page>
    PARENT_SCOPE)
