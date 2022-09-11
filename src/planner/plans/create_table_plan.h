#pragma once

#include <string>

#include "plan_node.h"
#include "../../storage/catalog/catalog_table.h"
#include "../../parser/statement/create_table.h"

class CreateTablePlan : public PlanNode {
public:
    CatalogTable * table;
    CreateTablePlan(CreateTable * create_table_statement);
};
