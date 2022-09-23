#include "nested_join_executor.h"
#include "executor_factory.h"

NestedJoinExecutor::NestedJoinExecutor(const NestedJoinPlan * plan) : plan{plan} {}

std::vector<std::vector<AbstractData*>> NestedJoinExecutor::Execute(Catalog * catalog) {
    std::vector<std::vector<AbstractData*>> left_rows = ExecutorFactory::CreateExecutor(plan->left).get()->Execute(catalog);    
    std::vector<std::vector<AbstractData*>> right_rows = ExecutorFactory::CreateExecutor(plan->right).get()->Execute(catalog);

    std::vector<std::vector<AbstractData*>> res_rows;
    const Scope scope = plan->GetScope();

    for (std::vector<AbstractData*> left_row : left_rows) {
        for (std::vector<AbstractData*> right_row : right_rows) {
            std::vector<AbstractData*> res_row;

            for (AbstractData* data : left_row) {
                res_row.emplace_back(data);
            }
            for (AbstractData* data : right_row) {
                res_row.emplace_back(data);
            }

            if (plan->predicate->Evaluate(scope, res_row)->IsTruthy()) {
                res_rows.emplace_back(res_row);
            }
        }
    }

    return res_rows;
}
