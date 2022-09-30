#include "delete_plan.h"

DeletePlan::DeletePlan(std::string table, Expression * where, Catalog * catalog) : PlanNode{PlanType::Delete, catalog}, table{table}, where{where} {}

Scope DeletePlan::GetScope() const {
    if (catalog->ReadTable(table).isErr()) {
        throw Error{ErrorType::Internal, "Table does not exist."};
    }

    Scope s{catalog->ReadTable(table).unwrap()->GetNumberOfColumns()};
    
    CatalogTable * catalog_table = catalog->ReadTable(table).unwrap();
    std::vector<CatalogColumn*> columns = catalog_table->GetColumns();

    for (int i = 0; i < columns.size(); i++) {
        s.AddFieldToScope(catalog_table->GetTableName() + "." + columns.at(i)->GetColumnName(), i);
    }

    return s;
}
