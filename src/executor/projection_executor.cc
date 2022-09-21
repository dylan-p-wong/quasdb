#include "projection_executor.h"
#include "executor_factory.h"

ProjectionExecutor::ProjectionExecutor(const ProjectionPlan * plan) : plan{plan} {}

std::vector<std::vector<AbstractData*>> ProjectionExecutor::Execute(Catalog * catalog) {
    std::vector<std::vector<AbstractData*>> rows = ExecutorFactory::CreateExecutor(plan->source).get()->Execute(catalog);
    
    std::vector<std::vector<AbstractData*>> res_rows;
    const Scope scope = plan->GetScope();

    for (std::vector<AbstractData*> row : rows) {
        std::vector<AbstractData*> res_row;
        for (Expression * expression : plan->expressions) {
            res_row.emplace_back(expression->Evaluate(scope, row).release());
        }
        res_rows.emplace_back(res_row);
    }

    return res_rows;
}
