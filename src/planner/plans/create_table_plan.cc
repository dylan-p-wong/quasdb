#include "create_table_plan.h"

CreateTablePlan::CreateTablePlan(CreateTable * create_table_statement) : PlanNode{PlanType::CreateTable} {
    CatalogTable * catalog_table = new CatalogTable{};
    catalog_table->name = create_table_statement->name;
    std::vector<CatalogColumn*> columns;

    for (const Column & c : create_table_statement->columns) {
        columns.push_back(new CatalogColumn{c});
    }

    catalog_table->columns = columns;
    
    table = catalog_table;
}
