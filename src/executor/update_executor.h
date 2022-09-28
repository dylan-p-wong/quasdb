#pragma once

#include "abstract_executor.h"
#include "../planner/plans/plan_node.h"
#include "../planner/plans/update_plan.h"

class UpdateExecutor : public AbstractExecutor {
public:
    const UpdatePlan * plan;
    UpdateExecutor(const UpdatePlan * plan);
    std::vector<std::vector<AbstractData*>> Execute(Catalog * catalog) override;
};
