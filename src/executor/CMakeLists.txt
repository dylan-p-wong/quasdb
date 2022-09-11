
add_library(executor executor.h executor.cc abstract_executor.h abstract_executor.cc executor_factory.h executor_factory.cc create_table_executor.h create_table_executor.cc drop_table_executor.h drop_table_executor.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:executor>
    PARENT_SCOPE)
