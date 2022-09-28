#pragma once

#include "abstract_executor.h"
#include "../planner/plans/plan_node.h"
#include "../planner/plans/order_plan.h"
#include "../storage/table/tuple.h"

class OrderExecutor : public AbstractExecutor {
    const OrderPlan * plan;
public:
    OrderExecutor(const OrderPlan * plan);
    std::vector<std::vector<AbstractData*>> Execute(Catalog * catalog) override;
};
