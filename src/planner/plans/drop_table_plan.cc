#include "drop_table_plan.h"

DropTablePlan::DropTablePlan(DropTable * ast) : PlanNode{PlanType::DropTable}, table{ast->name} {}
