#pragma once

#include "abstract_executor.h"
#include "../planner/plans/plan_node.h"
#include "../planner/plans/sequential_scan_plan.h"
#include "../storage/table/tuple.h"

class SequentialScanExecutor : public AbstractExecutor {
    const SequentialScanPlan * plan;
public:
    SequentialScanExecutor(const SequentialScanPlan * plan);
    ExecutionOutput Execute(Catalog * catalog) override;
    std::vector<AbstractData*> Next();
};
