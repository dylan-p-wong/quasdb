add_library(plans plan_node.h plan_node.cc create_table_plan.h create_table_plan.cc drop_table_plan.h drop_table_plan.cc insert_plan.h insert_plan.cc sequential_scan_plan.h sequential_scan_plan.cc projection_plan.h projection_plan.cc filter_plan.h filter_plan.cc nested_join_plan.h nested_join_plan.cc delete_plan.h delete_plan.cc update_plan.h update_plan.cc order_plan.h order_plan.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:plans>
    PARENT_SCOPE)
