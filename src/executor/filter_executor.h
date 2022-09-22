#pragma once

#include "abstract_executor.h"
#include "../planner/plans/plan_node.h"
#include "../planner/plans/filter_plan.h"
#include "../storage/table/tuple.h"

class FilterExecutor : public AbstractExecutor {
    const FilterPlan * plan;
public:
    FilterExecutor(const FilterPlan * plan);
    // ExecutionOutput Execute(Catalog * catalog) override;
    std::vector<std::vector<AbstractData*>> Execute(Catalog * catalog) override;
};
