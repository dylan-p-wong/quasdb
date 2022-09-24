#include "planner.h"

#include "../parser/statement/create_table.h"
#include "../parser/statement/drop_table.h"
#include "../parser/statement/insert.h"
#include "../parser/statement/select.h"
#include "../parser/statement/delete.h"

#include "./plans/create_table_plan.h"
#include "./plans/drop_table_plan.h"
#include "./plans/insert_plan.h"
#include "./plans/sequential_scan_plan.h"
#include "./plans/filter_plan.h"
#include "./plans/projection_plan.h"
#include "./plans/nested_join_plan.h"
#include "./plans/delete_plan.h"


std::unique_ptr<PlanNode> Planner::CreatePlan(Statement * ast) {
    try {
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
                std::unique_ptr<PlanNode> node = BuildFromNodePlan(select_statement->from.at(0));
                for (int i = 1; i < select_statement->from.size(); i++) {
                    std::unique_ptr<PlanNode> right = BuildFromNodePlan(select_statement->from.at(i));
                    node = std::make_unique<NestedJoinPlan>(node.release(), right.release(), nullptr, JoinType::Outer, catalog); // should not use release but temp
                }

                //  build where clause
                if (select_statement->where) {
                    node = std::make_unique<FilterPlan>(node.release(), select_statement->where, catalog); // should not use release but temp
                }

                // build select clause
                if (select_statement->select.size()) {
                    node = std::make_unique<ProjectionPlan>(node.release(), select_statement->select, catalog); // should not use release but temp
                }

                // build having clause
                
                // build order clause

                // build offset clause

                // build limit clause
                
                // return std::make_unique<SequentialScanPlan>(dynamic_cast<TableFromItem*>(select_statement->from.at(0))->name, catalog);
                return std::move(node);
            }
            case StatementType::Delete: {
                DeleteStatement * delete_statement = dynamic_cast<DeleteStatement*>(ast);

                // build from clause
                std::unique_ptr<PlanNode> node = BuildFromNodePlan(new TableFromItem{delete_statement->table, ""});

                // build where clause
                if (delete_statement->where) {
                    node = std::make_unique<FilterPlan>(node.release(), delete_statement->where, catalog); // should not use release but temp
                }

                // build delete node
                node = std::make_unique<DeletePlan>(node.release(), delete_statement->table, catalog);

                return std::move(node);
            }
            default:
                throw Error{ErrorType::Internal, "Statement type not found."};
        }
    } catch (...) {
        return nullptr;
    }
}

std::unique_ptr<PlanNode> Planner::BuildFromNodePlan(FromItem * from_item) {
    if (from_item->type == FromType::Table) {
        if (dynamic_cast<TableFromItem*>(from_item)->alias.size() > 0) {
            return std::make_unique<SequentialScanPlan>(dynamic_cast<TableFromItem*>(from_item)->name, dynamic_cast<TableFromItem*>(from_item)->alias, catalog);
        }
        return std::make_unique<SequentialScanPlan>(dynamic_cast<TableFromItem*>(from_item)->name, catalog);
    } else if (from_item->type == FromType::Join) {
        JoinFromItem * j = dynamic_cast<JoinFromItem*>(from_item);
        std::unique_ptr<PlanNode> left = BuildFromNodePlan(j->left);
        std::unique_ptr<PlanNode> right = BuildFromNodePlan(j->right);

        return std::make_unique<NestedJoinPlan>(left.release(), right.release(), j->predicate, j->join_type, catalog); // should not use release but temp
    }
}
