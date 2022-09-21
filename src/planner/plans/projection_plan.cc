#include "projection_plan.h"

ProjectionPlan::ProjectionPlan(PlanNode * source, std::vector<SelectItem*> select, Catalog * catalog) : PlanNode{PlanType::Projection, catalog}, source{source} {
    std::vector<Expression*> expressions_;
    for (int i = 0; i < select.size(); i++) {
        expressions_.push_back(select.at(i)->field);
    }

    expressions = std::move(expressions_);
}

Scope ProjectionPlan::GetScope() const {
    return source->GetScope();
}
