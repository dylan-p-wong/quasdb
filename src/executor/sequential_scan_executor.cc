#include "sequential_scan_executor.h"

SequentialScanExecutor::SequentialScanExecutor(const SequentialScanPlan * plan) : plan{plan} {}

std::vector<std::vector<AbstractData*>> SequentialScanExecutor::Execute(Catalog * catalog) {

    CatalogTable * table = catalog->ReadTable(plan->table).unwrap();

    std::vector<Tuple*> tuples = table->GetTuples(catalog->buffer_manager);

    std::vector<std::vector<AbstractData*>> rows;

    for (Tuple * tuple : tuples) {
        std::vector<AbstractData*> row;
        for (int i = 0; i < table->GetNumberOfColumns(); i++) {
            row.emplace_back(tuple->GetValueAtColumnIndex(i, table).release());
        }
        rows.emplace_back(row);
    }

    return rows;
}
