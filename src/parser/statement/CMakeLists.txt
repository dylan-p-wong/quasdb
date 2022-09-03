add_library(statement statement.h create_table.h drop_table.h expression.h expression.cc statement.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:statement>
    PARENT_SCOPE)
