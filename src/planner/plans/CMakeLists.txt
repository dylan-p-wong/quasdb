add_library(plans plan_node.h plan_node.cc create_table_plan.h create_table_plan.cc drop_table_plan.h drop_table_plan.cc insert_plan.h insert_plan.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:plans>
    PARENT_SCOPE)
