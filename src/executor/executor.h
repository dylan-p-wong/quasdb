#pragma once

#include "executor_factory.h"
#include "../planner/plans/plan_node.h"
#include "../common/execution_output.h"

class Executor {
    Catalog * catalog;
public:
    Executor(Catalog * catalog);
    ExecutionOutput Execute(const PlanNode * plan);
};
