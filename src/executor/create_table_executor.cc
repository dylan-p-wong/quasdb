#include "create_table_executor.h"

CreateTableExecutor::CreateTableExecutor(const CreateTablePlan * plan) : AbstractExecutor{}, plan{plan} {}

ExecutionOutput CreateTableExecutor::Execute(Catalog * catalog) {
    ExecutionOutput res{};

    res.type = OutputType::CreateTable;

    auto create_catalog_table_result = catalog->CreateTable(plan->table);

    if (create_catalog_table_result.isOk()) {
        res.error = false;
    } else {
        res.error = true;
        res.error_type = create_catalog_table_result.unwrapErr().type;
        res.error_message = create_catalog_table_result.unwrapErr().message;
    }

    return res;
}
