#include "planner.h"

#include "../parser/statement/create_table.h"
#include "../parser/statement/drop_table.h"
#include "../parser/statement/insert.h"
#include "../parser/statement/select.h"

#include "./plans/create_table_plan.h"
#include "./plans/drop_table_plan.h"
#include "./plans/insert_plan.h"
#include "./plans/sequential_scan_plan.h"

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
        case StatementType::Select: {
            SelectStatement * select_statement = dynamic_cast<SelectStatement*>(ast);
            
            // build from clause
            if (select_statement->from.at(0)->type != FromType::Table) {
                throw; // joins not supported yet
            }

            // build where clause

            // build select clause

            // build having clause

            // build order clause

            // build offset clause

            // build limit clause
            
            return std::make_unique<SequentialScanPlan>(dynamic_cast<TableFromItem*>(select_statement->from.at(0))->name);
        }
        default:
            throw;
    }
}
