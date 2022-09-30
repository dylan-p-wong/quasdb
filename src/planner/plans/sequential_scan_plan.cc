#include "sequential_scan_plan.h"

SequentialScanPlan::SequentialScanPlan(std::string table, Catalog * catalog) : PlanNode{PlanType::SequentialScan, catalog}, table{table} {
    if (catalog->ReadTable(table).isErr()) {
        throw Error{ErrorType::Internal, "Table does not exist."};
    }
}

SequentialScanPlan::SequentialScanPlan(std::string table, std::string alias, Catalog * catalog) : PlanNode{PlanType::SequentialScan, catalog}, has_alias{true}, alias{alias}, table{table} {
    if (catalog->ReadTable(table).isErr()) {
        throw Error{ErrorType::Internal, "Table does not exist."};  
    }
}

Scope SequentialScanPlan::GetScope() const {
    if (catalog->ReadTable(table).isErr()) {
        throw Error{ErrorType::Internal, "Table does not exist."};
    }

    Scope s{catalog->ReadTable(table).unwrap()->GetNumberOfColumns()};
    
    CatalogTable * catalog_table = catalog->ReadTable(table).unwrap();
    std::vector<CatalogColumn*> columns = catalog_table->GetColumns();

    for (int i = 0; i < columns.size(); i++) {
        if (has_alias) {
            s.AddFieldToScope(alias + "." + columns.at(i)->GetColumnName(), i);
        } else {
            s.AddFieldToScope(catalog_table->GetTableName() + "." + columns.at(i)->GetColumnName(), i);
        }
    }

    return s;
}
