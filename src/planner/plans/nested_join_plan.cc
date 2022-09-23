#include "nested_join_plan.h"

NestedJoinPlan::NestedJoinPlan(PlanNode * left, PlanNode * right, Expression * predicate, Catalog * catalog) : PlanNode{PlanType::NestedLoopJoin, catalog}, left{left}, right{right}, predicate{predicate} {}

Scope NestedJoinPlan::GetScope() const {
    return left->GetScope().Merge(right->GetScope());
}
