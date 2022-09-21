#pragma once

#include <string>
#include <vector>

#include "plan_node.h"
#include "../../parser/statement/insert.h"
#include "../../common/shared.h"

class FilterPlan : public PlanNode {
public:
    PlanNode * source;
    Expression* expression;
    FilterPlan(PlanNode*, Expression*);
};
