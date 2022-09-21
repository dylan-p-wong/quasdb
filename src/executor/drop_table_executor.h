#pragma once

#include "abstract_executor.h"
#include "../planner/plans/plan_node.h"
#include "../planner/plans/drop_table_plan.h"

class DropTableExecutor : public AbstractExecutor {
public:
    const DropTablePlan * plan;
    DropTableExecutor(const DropTablePlan * plan);
    std::vector<std::vector<AbstractData*>> Execute(Catalog * catalog) override;
};
