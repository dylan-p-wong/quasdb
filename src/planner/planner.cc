#include "planner.h"

#include "../parser/statement/create_table.h"
#include "../parser/statement/drop_table.h"
#include "../parser/statement/insert.h"
#include "../parser/statement/select.h"

#include "./plans/create_table_plan.h"
#include "./plans/drop_table_plan.h"
#include "./plans/insert_plan.h"
#include "./plans/sequential_scan_plan.h"
#include "./plans/filter_plan.h"
#include "./plans/projection_plan.h"

std::unique_ptr<PlanNode> Planner::CreatePlan(Statement * ast) {
    switch (ast->type) {
        case StatementType::CreateTable: {
            CreateTable * create_table_statement = dynamic_cast<CreateTable*>(ast);
            return std::make_unique<CreateTablePlan>(create_table_statement, catalog);
        }
        case StatementType::DropTable: {
            DropTable * drop_table_statement = dynamic_cast<DropTable*>(ast);
            return std::make_unique<DropTablePlan>(drop_table_statement, catalog);
        }
        case StatementType::Insert: {
            InsertStatement * insert_table_statement = dynamic_cast<InsertStatement*>(ast);
            return std::make_unique<InsertPlan>(insert_table_statement, catalog);
        }
        case StatementType::Select: {
            SelectStatement * select_statement = dynamic_cast<SelectStatement*>(ast);
            
            // build from clause
            if (select_statement->from.at(0)->type != FromType::Table) {
                throw; // joins not supported yet
            }

            std::unique_ptr<PlanNode> node = std::make_unique<SequentialScanPlan>(dynamic_cast<TableFromItem*>(select_statement->from.at(0))->name, catalog);

            // PlanNode * node = new SequentialScanPlan{dynamic_cast<TableFromItem*>(select_statement->from.at(0))->name, catalog};

            //  build where clause
            if (select_statement->where) {
                node = std::make_unique<FilterPlan>(node.release(), select_statement->where, catalog); // should not use release but temp
            }

            // build select clause

            if (select_statement->select.size()) {
                // node = new ProjectionPlan{node, select_statement->select, catalog};
                node = std::make_unique<ProjectionPlan>(node.release(), select_statement->select, catalog); // should not use release but temp
            }

            // build having clause

            // build order clause

            // build offset clause

            // build limit clause
            
            // return std::make_unique<SequentialScanPlan>(dynamic_cast<TableFromItem*>(select_statement->from.at(0))->name, catalog);
            return std::move(node);
        }
        default:
            throw;
    }
}
