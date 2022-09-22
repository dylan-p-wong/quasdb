#pragma once

#include "filter_executor.h"
#include "executor_factory.h"

FilterExecutor::FilterExecutor(const FilterPlan * plan) : plan{plan} {}

std::vector<std::vector<AbstractData*>> FilterExecutor::Execute(Catalog * catalog) {    
    std::vector<std::vector<AbstractData*>> rows = ExecutorFactory::CreateExecutor(plan->source).get()->Execute(catalog);
    std::vector<std::vector<AbstractData*>> res_rows;

    const Scope scope = plan->GetScope();

    for (std::vector<AbstractData*> row : rows) {
        if (plan->expression->Evaluate(scope, row)->IsTruthy()) {
            res_rows.emplace_back(row);
        }
    }

    return res_rows;
}
