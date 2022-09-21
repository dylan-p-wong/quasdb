#pragma once

#include <string>

#include "plan_node.h"
#include "../../parser/statement/drop_table.h"

class DropTablePlan : public PlanNode {
public:
    std::string table;
    DropTablePlan(DropTable * ast, Catalog * catalog);
};
