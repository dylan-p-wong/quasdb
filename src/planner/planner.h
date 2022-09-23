#pragma once

#include <memory>

#include "../storage/catalog/catalog.h"
#include "../parser/statement/statement.h"
#include "../parser/statement/select.h"
#include "plans/plan_node.h"

class Planner {
    Catalog * catalog;
public:
    Planner(Catalog * catalog) : catalog{catalog} {}
    std::unique_ptr<PlanNode> CreatePlan(Statement * ast);
    std::unique_ptr<PlanNode> BuildFromNodePlan(FromItem * from_item);
};
