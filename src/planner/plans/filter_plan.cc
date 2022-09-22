#include "filter_plan.h"

FilterPlan::FilterPlan(PlanNode * source, Expression * expression, Catalog * catalog) : PlanNode{PlanType::Filter, catalog}, source{source}, expression{expression} {}

Scope FilterPlan::GetScope() const {
    return source->GetScope();
}
