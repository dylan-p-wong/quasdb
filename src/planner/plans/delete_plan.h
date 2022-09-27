#pragma once

#include <string>
#include <vector>

#include "plan_node.h"
#include "../../parser/statement/delete.h"
#include "../../common/shared.h"

class DeletePlan : public PlanNode {
public:
    std::string table;
    Expression * where;
    DeletePlan(std::string table, Expression * where, Catalog * catalog);
    Scope GetScope() const override;
};
