#include "nested_join_plan.h"

NestedJoinPlan::NestedJoinPlan(PlanNode * left, PlanNode * right, Expression * predicate, JoinType join_type, Catalog * catalog) : PlanNode{PlanType::NestedLoopJoin, catalog}, left{left}, right{right}, predicate{predicate}, join_type{join_type} {}

Scope NestedJoinPlan::GetScope() const {
    return left->GetScope().Merge(right->GetScope());
}
