#include "drop_table_plan.h"

DropTablePlan::DropTablePlan(DropTable * ast, Catalog * catalog) : PlanNode{PlanType::DropTable, catalog}, table{ast->name} {}
