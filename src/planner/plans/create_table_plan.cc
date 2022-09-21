#include "create_table_plan.h"

CreateTablePlan::CreateTablePlan(CreateTable * create_table_statement, Catalog * catalog) : PlanNode{PlanType::CreateTable, catalog} {
    CatalogTable * catalog_table = new CatalogTable{};
    catalog_table->name = create_table_statement->name;
    std::vector<CatalogColumn*> columns;

    int current_tuple_size = 0;
    for (const Column & c : create_table_statement->columns) {
        CatalogColumn * new_column = new CatalogColumn{c, current_tuple_size};
        current_tuple_size += new_column->GetColumnSize();
        columns.push_back(new_column);
    }

    catalog_table->columns = columns;
    catalog_table->tuple_size = current_tuple_size;
    
    table = catalog_table;
}
