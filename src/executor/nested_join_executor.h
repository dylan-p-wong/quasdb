#pragma once

#include "abstract_executor.h"
#include "../planner/plans/plan_node.h"
#include "../planner/plans/nested_join_plan.h"
#include "../storage/table/tuple.h"

class NestedJoinExecutor : public AbstractExecutor {
    const NestedJoinPlan * plan;
public:
    NestedJoinExecutor(const NestedJoinPlan * plan);
    // ExecutionOutput Execute(Catalog * catalog) override;
    std::vector<std::vector<AbstractData*>> Execute(Catalog * catalog) override;
};
