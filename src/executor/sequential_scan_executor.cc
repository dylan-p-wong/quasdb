#include "sequential_scan_executor.h"

SequentialScanExecutor::SequentialScanExecutor(const SequentialScanPlan * plan) : plan{plan} {}

ExecutionOutput SequentialScanExecutor::Execute(Catalog * catalog) {
    ExecutionOutput res{};
    res.type = OutputType::Select;

    if (catalog->ReadTable(plan->table).isErr()) {
        res.error = true;
        return res;
    }

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

    res.rows = rows;

    return res;
}
