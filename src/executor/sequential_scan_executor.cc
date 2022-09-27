#include "sequential_scan_executor.h"

SequentialScanExecutor::SequentialScanExecutor(const SequentialScanPlan * plan) : plan{plan} {}

std::vector<std::vector<AbstractData*>> SequentialScanExecutor::Execute(Catalog * catalog) {

    CatalogTable * table = catalog->ReadTable(plan->table).unwrap();

    std::vector<OutputTuple*> tuples = table->GetTuples(catalog->GetCatalogBufferManager());

    std::vector<std::vector<AbstractData*>> rows;

    for (OutputTuple * tuple : tuples) {
        rows.emplace_back(tuple->GetAsValues(table));
    }

    return rows;
}
