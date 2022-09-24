#pragma once

#include "abstract_executor.h"
#include "../planner/plans/plan_node.h"
#include "../planner/plans/delete_plan.h"

class DeleteExecutor : public AbstractExecutor {
public:
    const DeletePlan * plan;
    DeleteExecutor(const DeletePlan * plan);
    std::vector<std::vector<AbstractData*>> Execute(Catalog * catalog) override;
};
