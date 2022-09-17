#pragma once

#include "abstract_executor.h"
#include "../planner/plans/plan_node.h"
#include "../planner/plans/insert_plan.h"

class InsertExecutor : public AbstractExecutor {
public:
    const InsertPlan * plan;
    InsertExecutor(const InsertPlan * plan);
    ExecutionOutput Execute(Catalog * catalog) override;
};
