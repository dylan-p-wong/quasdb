#include "delete_executor.h"
#include "executor_factory.h"

DeleteExecutor::DeleteExecutor(const DeletePlan * plan) : AbstractExecutor{}, plan{plan} {}

std::vector<std::vector<AbstractData*>> DeleteExecutor::Execute(Catalog * catalog) {
    CatalogTable * table = catalog->ReadTable(plan->table).unwrap();
    std::vector<Tuple*> tuples = table->GetTuples(catalog->GetCatalogBufferManager());

    for (Tuple * tuple : tuples) {
        if (plan->where && !plan->where->Evaluate(plan->GetScope(), tuple->GetAsValues(table))->IsTruthy()) {
            continue;
        }
        table->MarkDelete(tuple->GetRID(), catalog->GetCatalogBufferManager());
    }

    std::vector<std::vector<AbstractData*>> res;
    return res;
}
