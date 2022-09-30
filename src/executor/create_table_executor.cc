#include "create_table_executor.h"

CreateTableExecutor::CreateTableExecutor(const CreateTablePlan * plan) : AbstractExecutor{}, plan{plan} {}

std::vector<std::vector<AbstractData*>> CreateTableExecutor::Execute(Catalog * catalog) {

    auto create_catalog_table_result = catalog->CreateTable(plan->table);

    if (!create_catalog_table_result.isOk()) {
        throw Error{ErrorType::Internal, "Error creating table."};
    }

    std::vector<std::vector<AbstractData*>> res;
    return res;
}
