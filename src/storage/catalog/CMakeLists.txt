add_library(catalog catalog.h catalog.cc catalog_table.h catalog_table.cc catalog_column.h catalog_column.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:catalog>
    PARENT_SCOPE)
