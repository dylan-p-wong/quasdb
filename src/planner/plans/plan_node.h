#pragma once

#include <vector>

#include "../../storage/catalog/catalog.h"

enum class PlanType {
    CreateTable,
    DropTable,
    //
    SequentialScan,
    IndexScan,
    Insert,
    Update,
    Delete,
    Aggregation,
    Limit,
    Distinct,
    NestedLoopJoin,
    NestedIndexJoin,
    HashJoin
};

class PlanNode {
public:
    // Schema for the output of the plan node
    std::vector<PlanNode*> children;
    PlanType type;
    PlanNode(PlanType type);
    virtual ~PlanNode() = 0;
};
