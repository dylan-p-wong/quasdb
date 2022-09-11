#pragma once

#include <memory>

#include "../storage/catalog/catalog.h"
#include "../parser/statement/statement.h"
#include "plans/plan_node.h"

class Planner {
public:
    std::unique_ptr<PlanNode> CreatePlan(Statement * ast);
};
