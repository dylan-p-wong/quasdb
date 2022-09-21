#pragma once

#include "abstract_executor.h"
#include "../planner/plans/plan_node.h"
#include "../planner/plans/projection_plan.h"
#include "../storage/table/tuple.h"

class ProjectionExecutor : public AbstractExecutor {
    const ProjectionPlan * plan;
public:
    ProjectionExecutor(const ProjectionPlan * plan);
    // ExecutionOutput Execute(Catalog * catalog) override;
    std::vector<std::vector<AbstractData*>> Execute(Catalog * catalog) override;
};
