#include "order_plan.h"

OrderPlan::OrderPlan(PlanNode * source, std::vector<OrderItem*> order_by, Catalog * catalog) : PlanNode{PlanType::Order, catalog}, source{source}, order_by{order_by} {}

Scope OrderPlan::GetScope() const {
    return source->GetScope();
}
