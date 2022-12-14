#include "insert_plan.h"
#include "../../parser/statement/expression.h"

InsertPlan::InsertPlan(InsertStatement * ast, Catalog * catalog) : PlanNode{PlanType::Insert, catalog}, table{ast->table}, columns{ast->columns} {
    std::vector<std::vector<std::unique_ptr<AbstractData>>> new_values;
    
    for (std::vector<Expression*> row : ast->values) {
        std::vector<std::unique_ptr<AbstractData>> new_row;
        for (Expression * value : row) {
            std::unique_ptr<AbstractData> value_constant = value->EvaluateConstant();

            new_row.push_back(std::move(value_constant));
        }
        new_values.push_back(std::move(new_row));
    }

    values = std::move(new_values);
}
