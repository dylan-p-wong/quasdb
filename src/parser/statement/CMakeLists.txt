add_library(statement statement.h statement.cc create_table.h drop_table.h select.h select.cc insert.h insert.cc delete.h delete.cc expression.h expression.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:statement>
    PARENT_SCOPE)
