#include "update_executor.h"
#include "executor_factory.h"

UpdateExecutor::UpdateExecutor(const UpdatePlan * plan) : AbstractExecutor{}, plan{plan} {}

std::vector<std::vector<AbstractData*>> UpdateExecutor::Execute(Catalog * catalog) {
    CatalogTable * table = catalog->ReadTable(plan->table).unwrap();
    std::vector<OutputTuple*> tuples = table->GetTuples(catalog->GetCatalogBufferManager());

    for (OutputTuple * tuple : tuples) {
        if (plan->where && !plan->where->Evaluate(plan->GetScope(), tuple->GetAsValues(table))->IsTruthy()) {
            continue;
        }

        std::unordered_map<std::string, AbstractData*> constant_evaluated_set;

        for (auto& it: plan->set) {
            constant_evaluated_set[it.first] = it.second->EvaluateConstant().release();
        }

        if (!table->ValidateSet(constant_evaluated_set)) {
            throw Error{ErrorType::Internal, "Invalid values."}; 
        }

        auto tuple_res = table->UpdateTuple(tuple->GetRID(), constant_evaluated_set, catalog->GetCatalogBufferManager());

        if (tuple_res.isErr()) {
            throw tuple_res.unwrapErr();
        }
    }

    std::vector<std::vector<AbstractData*>> res;
    return res;
}
