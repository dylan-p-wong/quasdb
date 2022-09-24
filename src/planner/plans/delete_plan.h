#pragma once

#include <string>
#include <vector>

#include "plan_node.h"
#include "../../parser/statement/delete.h"
#include "../../common/shared.h"

class DeletePlan : public PlanNode {
public:
    std::string table;
    PlanNode * source;
    DeletePlan(PlanNode * source, std::string table, Catalog * catalog);
    Scope GetScope() const override;
};
