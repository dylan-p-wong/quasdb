#include "executor_factory.h"

#include "create_table_executor.h"
#include "drop_table_executor.h"
#include "insert_executor.h"
#include "sequential_scan_executor.h"

#include "../planner/plans/create_table_plan.h"
#include "../planner/plans/drop_table_plan.h"
#include "../planner/plans/insert_plan.h"
#include "../planner/plans/sequential_scan_plan.h"

std::unique_ptr<AbstractExecutor> ExecutorFactory::CreateExecutor(const PlanNode * plan) {

    switch (plan->type) {
        case PlanType::CreateTable: {
            const CreateTablePlan * create_table_plan = dynamic_cast<const CreateTablePlan*>(plan);
            return std::make_unique<CreateTableExecutor>(create_table_plan);
        }
        case PlanType::DropTable: {
            const DropTablePlan * drop_table_plan = dynamic_cast<const DropTablePlan*>(plan);
            return std::make_unique<DropTableExecutor>(drop_table_plan);
        }
        case PlanType::Insert: {
            const InsertPlan * insert_plan = dynamic_cast<const InsertPlan*>(plan);
            return std::make_unique<InsertExecutor>(insert_plan);
        }
        case PlanType::SequentialScan: {
            const SequentialScanPlan * insert_plan = dynamic_cast<const SequentialScanPlan*>(plan);
            return std::make_unique<SequentialScanExecutor>(insert_plan);
        }
        default: {
            // Error unsupported plan
            throw;
        }
    }
}
