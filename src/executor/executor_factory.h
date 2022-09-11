#pragma once

#include <memory>

#include "../planner/plans/plan_node.h"
#include "abstract_executor.h"

class ExecutorFactory {
public:
    static std::unique_ptr<AbstractExecutor> CreateExecutor(const PlanNode * plan);
};
