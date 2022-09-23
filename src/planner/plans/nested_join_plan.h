#pragma once

#include <string>
#include <vector>

#include "../../parser/statement/select.h"
#include "plan_node.h"
#include "../../common/shared.h"
#include "../scope.h"

class NestedJoinPlan : public PlanNode {
public:
    PlanNode * left;
    PlanNode * right;
    Expression * predicate;
    JoinType join_type;
    NestedJoinPlan(PlanNode * left, PlanNode * right, Expression * predicate, JoinType join_type, Catalog * catalog);
    Scope GetScope() const override;
};
