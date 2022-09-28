
add_library(executor executor.h executor.cc abstract_executor.h abstract_executor.cc executor_factory.h executor_factory.cc create_table_executor.h create_table_executor.cc drop_table_executor.h drop_table_executor.cc insert_executor.h insert_executor.cc sequential_scan_executor.h sequential_scan_executor.cc projection_executor.h projection_executor.cc filter_executor.h filter_executor.cc nested_join_executor.h nested_join_executor.cc delete_executor.h delete_executor.cc update_executor.h update_executor.cc order_executor.h order_executor.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:executor>
    PARENT_SCOPE)
