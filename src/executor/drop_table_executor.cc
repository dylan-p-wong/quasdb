#include "drop_table_executor.h"

DropTableExecutor::DropTableExecutor(const DropTablePlan * plan) : AbstractExecutor{}, plan{plan} {}

ExecutionOutput DropTableExecutor::Execute(Catalog * catalog) {
    ExecutionOutput res{};

    res.type = OutputType::DropTable;

    auto delete_catalog_table_result = catalog->DeleteTable(plan->table);

    if (delete_catalog_table_result.isOk()) {
        res.error = false;
    } else {
        res.error = true;
        res.error_type = delete_catalog_table_result.unwrapErr().type;
        res.error_message = delete_catalog_table_result.unwrapErr().message;
    }

    return res;
}
