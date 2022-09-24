#include "delete_plan.h"

DeletePlan::DeletePlan(PlanNode * source, std::string table, Catalog * catalog) : PlanNode{PlanType::Delete, catalog}, table{table}, source{source} {}

Scope DeletePlan::GetScope() const {
    return source->GetScope();
}
