#pragma once

#include <string>
#include <vector>

#include "plan_node.h"
#include "../../parser/statement/delete.h"
#include "../../common/shared.h"

class UpdatePlan : public PlanNode {
public:
    std::unordered_map<std::string, Expression*> set;
    std::string table;
    Expression * where;
    UpdatePlan(std::string table, Expression * where, std::unordered_map<std::string, Expression*> set, Catalog * catalog);
    Scope GetScope() const override;
};
