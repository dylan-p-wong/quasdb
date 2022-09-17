#include "insert_executor.h"

InsertExecutor::InsertExecutor(const InsertPlan * plan) : AbstractExecutor{}, plan{plan} {}

ExecutionOutput InsertExecutor::Execute(Catalog * catalog) {
    ExecutionOutput res{};
    res.type = OutputType::Insert;

    if (catalog->ReadTable(plan->table).isErr()) {
        res.error = true;
        return res;
    }

    // Validation
    CatalogTable * table = catalog->ReadTable(plan->table).unwrap();

    if (plan->columns.size() == 0) {
        for (int i = 0; i < plan->values.size(); i++) {
            if (!table->ValidateRow(plan->values.at(i))) {
                res.error = true;
                return res;
            }
        }
    } else {
        // TODO(dylan) : Validate other tuples
    }

    // Insertion hardcoded data to start
    for (int i = 0; i < plan->values.size(); i++) {
        Tuple t{plan->values.at(i), table};
        table->InsertTuple(t, catalog->buffer_manager);
    }

    return res;
}
