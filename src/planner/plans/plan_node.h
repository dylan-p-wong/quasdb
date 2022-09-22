#pragma once

#include <vector>

#include "../../storage/catalog/catalog.h"
#include "../scope.h"

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
    HashJoin,
    Projection,
    Filter
};

class PlanNode {
public:
    Catalog * catalog;
    // Schema for the output of the plan node
    // std::vector<PlanNode*> children;
    PlanType type;
    PlanNode(PlanType type, Catalog * catalog);
    virtual Scope GetScope() const { Scope s; return s; }
    virtual ~PlanNode() = 0;
};
