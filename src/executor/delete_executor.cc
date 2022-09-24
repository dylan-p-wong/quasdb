#include "delete_executor.h"
#include "executor_factory.h"

DeleteExecutor::DeleteExecutor(const DeletePlan * plan) : AbstractExecutor{}, plan{plan} {}

std::vector<std::vector<AbstractData*>> DeleteExecutor::Execute(Catalog * catalog) {
    std::vector<std::vector<AbstractData*>> rows = ExecutorFactory::CreateExecutor(plan->source).get()->Execute(catalog);

    // TODO() Still need to delete. Need RID.

    std::vector<std::vector<AbstractData*>> res;
    return res;
}
