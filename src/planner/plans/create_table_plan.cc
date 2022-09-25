#include "create_table_plan.h"

CreateTablePlan::CreateTablePlan(CreateTable * create_table_statement, Catalog * catalog) : PlanNode{PlanType::CreateTable, catalog} {
    std::vector<CatalogColumn*> columns;

    int current_tuple_size = 0;
    for (const Column & c : create_table_statement->columns) {
        CatalogColumn * new_column = new CatalogColumn{c, current_tuple_size};
        current_tuple_size += new_column->GetColumnSize();
        columns.push_back(new_column);
    }
    
    table = new CatalogTable{create_table_statement->name, columns, current_tuple_size};
}
