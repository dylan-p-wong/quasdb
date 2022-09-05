add_library(statement statement.h statement.cc create_table.h drop_table.h expression.h expression.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:statement>
    PARENT_SCOPE)
