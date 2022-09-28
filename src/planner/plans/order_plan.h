#pragma once

#include <string>
#include <vector>

#include "plan_node.h"
#include "../../parser/statement/select.h"
#include "../../common/shared.h"

class OrderPlan : public PlanNode {
public:
    PlanNode * source;
    std::vector<OrderItem*> order_by;
    OrderPlan(PlanNode * source, std::vector<OrderItem*> order_by, Catalog * catalog);
    Scope GetScope() const override;
};
