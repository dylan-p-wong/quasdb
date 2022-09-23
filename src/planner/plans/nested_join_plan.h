#pragma once

#include <string>
#include <vector>

#include "plan_node.h"
#include "../../common/shared.h"
#include "../scope.h"

class NestedJoinPlan : public PlanNode {
public:
    PlanNode * left;
    PlanNode * right;
    Expression * predicate;
    NestedJoinPlan(PlanNode * left, PlanNode * right, Expression * predicate, Catalog * catalog);
    Scope GetScope() const override;
};
