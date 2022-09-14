#include "planner.h"

#include "../parser/statement/create_table.h"
#include "../parser/statement/drop_table.h"
#include "../parser/statement/insert.h"

#include "./plans/create_table_plan.h"
#include "./plans/drop_table_plan.h"
#include "./plans/insert_plan.h"

std::unique_ptr<PlanNode> Planner::CreatePlan(Statement * ast) {
    switch (ast->type) {
        case StatementType::CreateTable: {
            CreateTable * create_table_statement = dynamic_cast<CreateTable*>(ast);
            return std::make_unique<CreateTablePlan>(create_table_statement);
        }
        case StatementType::DropTable: {
            DropTable * drop_table_statement = dynamic_cast<DropTable*>(ast);
            return std::make_unique<DropTablePlan>(drop_table_statement);
        }
        case StatementType::Insert: {
            InsertStatement * insert_table_statement = dynamic_cast<InsertStatement*>(ast);
            return std::make_unique<InsertPlan>(insert_table_statement);
        }
        default:
            throw;
    }
}
