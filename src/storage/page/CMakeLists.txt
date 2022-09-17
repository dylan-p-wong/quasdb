add_library(page page.h page.cc directory_page.h directory_page.cc table_page.h table_page.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:page>
    PARENT_SCOPE)
