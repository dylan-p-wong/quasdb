#include "executor_factory.h"
#include "create_table_executor.h"
#include "drop_table_executor.h"

#include "../planner/plans/create_table_plan.h"
#include "../planner/plans/drop_table_plan.h"

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
        // case PlanType::SequentialScan: {
        //     std::unique_ptr<Executor> child_executor = ExecutorFactory::CreateExecutor(plan->children.at(0));
        //     return std::make_unique<CreateTableExecutor>(plan);
        // }
        default: {
            // Error unsupported plan
            throw;
        }
    }
}
