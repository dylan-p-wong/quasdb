#include "sequential_scan_plan.h"

SequentialScanPlan::SequentialScanPlan(std::string table, Catalog * catalog) : PlanNode{PlanType::SequentialScan, catalog}, table{table} {
    if (catalog->ReadTable(table).isErr()) {
        throw;
    }
}

Scope SequentialScanPlan::GetScope() const {
    if (catalog->ReadTable(table).isErr()) {
        throw;
    }

    Scope s{catalog->ReadTable(table).unwrap()->GetNumberOfColumns()};
    
    CatalogTable * catalog_table = catalog->ReadTable(table).unwrap();
    std::vector<CatalogColumn*> columns = catalog_table->columns;

    for (int i = 0; i < columns.size(); i++) {
        s.AddFieldToScope(catalog_table->name + "." + columns.at(i)->name, i);
    }

    return s;
}
