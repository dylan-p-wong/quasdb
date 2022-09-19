#include "sequential_scan_plan.h"

SequentialScanPlan::SequentialScanPlan(std::string table) : PlanNode{PlanType::SequentialScan}, table{table} {}
