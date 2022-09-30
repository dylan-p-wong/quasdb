#include "update_plan.h"

UpdatePlan::UpdatePlan(std::string table, Expression * where, std::unordered_map<std::string, Expression*> set, Catalog * catalog) : PlanNode{PlanType::Update, catalog}, table{table}, where{where}, set{set} {}

Scope UpdatePlan::GetScope() const {
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
