#pragma once

#include <string>
#include <vector>

#include "plan_node.h"
#include "../../parser/statement/insert.h"
#include "../../common/shared.h"

class SequentialScanPlan : public PlanNode {
public:
    std::string table;
    SequentialScanPlan(std::string table);
};
