#include "drop_table_executor.h"

DropTableExecutor::DropTableExecutor(const DropTablePlan * plan) : AbstractExecutor{}, plan{plan} {}

std::vector<std::vector<AbstractData*>> DropTableExecutor::Execute(Catalog * catalog) {
    
    auto delete_catalog_table_result = catalog->DeleteTable(plan->table);

    if (!delete_catalog_table_result.isOk()) {
        throw Error{ErrorType::Internal, ""};
    }

    std::vector<std::vector<AbstractData*>> res;
    return res;
}
