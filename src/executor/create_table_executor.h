#pragma once

#include "abstract_executor.h"
#include "../planner/plans/plan_node.h"
#include "../planner/plans/create_table_plan.h"

class CreateTableExecutor : public AbstractExecutor {
public:
    const CreateTablePlan * plan;
    CreateTableExecutor(const CreateTablePlan * plan);
    std::vector<std::vector<AbstractData*>> Execute(Catalog * catalog) override;
};
