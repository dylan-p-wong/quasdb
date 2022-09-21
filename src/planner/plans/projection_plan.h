#pragma once

#include <string>
#include <vector>

#include "plan_node.h"
#include "../../parser/statement/select.h"
#include "../../common/shared.h"

class ProjectionPlan : public PlanNode {
public:
    PlanNode * source;
    std::vector<Expression*> expressions;
    ProjectionPlan(PlanNode * source, std::vector<SelectItem*> select, Catalog * catalog);
    Scope GetScope() const override;
};
