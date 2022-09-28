#include "executor_factory.h"

#include "create_table_executor.h"
#include "drop_table_executor.h"
#include "insert_executor.h"
#include "sequential_scan_executor.h"
#include "projection_executor.h"
#include "filter_executor.h"
#include "nested_join_executor.h"
#include "delete_executor.h"
#include "update_executor.h"

#include "../planner/plans/create_table_plan.h"
#include "../planner/plans/drop_table_plan.h"
#include "../planner/plans/insert_plan.h"
#include "../planner/plans/sequential_scan_plan.h"
#include "../planner/plans/projection_plan.h"
#include "../planner/plans/filter_plan.h"
#include "../planner/plans/nested_join_plan.h"
#include "../planner/plans/delete_plan.h"
#include "../planner/plans/update_plan.h"

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
        case PlanType::Delete: {
            const DeletePlan * delete_plan = dynamic_cast<const DeletePlan*>(plan);
            return std::make_unique<DeleteExecutor>(delete_plan);
        }
        case PlanType::Update: {
            const UpdatePlan * update_plan = dynamic_cast<const UpdatePlan*>(plan);
            return std::make_unique<UpdateExecutor>(update_plan);
        }
        case PlanType::SequentialScan: {
            const SequentialScanPlan * insert_plan = dynamic_cast<const SequentialScanPlan*>(plan);
            return std::make_unique<SequentialScanExecutor>(insert_plan);
        }
        case PlanType::Projection: {
            const ProjectionPlan * projection_plan = dynamic_cast<const ProjectionPlan*>(plan);
            return std::make_unique<ProjectionExecutor>(projection_plan);
        }
        case PlanType::Filter: {
            const FilterPlan * filter_plan = dynamic_cast<const FilterPlan*>(plan);
            return std::make_unique<FilterExecutor>(filter_plan);
        }
        case PlanType::NestedLoopJoin: {
            const NestedJoinPlan * nested_join_plan = dynamic_cast<const NestedJoinPlan*>(plan);
            return std::make_unique<NestedJoinExecutor>(nested_join_plan);
        }
        default: {
            // Error unsupported plan
            throw Error{ErrorType::Internal, "Unsupported plan."}; 
        }
    }
}
