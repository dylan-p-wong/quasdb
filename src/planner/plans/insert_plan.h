#pragma once

#include <string>
#include <vector>

#include "plan_node.h"
#include "../../parser/statement/insert.h"
#include "../../common/shared.h"

class InsertPlan : public PlanNode {
public:
    std::string table;
    std::vector<std::string> columns;
    std::vector<std::vector<std::unique_ptr<AbstractData>>> values;
    InsertPlan(InsertStatement * ast);
};
