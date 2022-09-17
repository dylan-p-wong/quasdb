#include "insert_executor.h"

InsertExecutor::InsertExecutor(const InsertPlan * plan) : AbstractExecutor{}, plan{plan} {}

ExecutionOutput InsertExecutor::Execute(Catalog * catalog) {
    ExecutionOutput res{};
    res.type = OutputType::Insert;

    if (catalog->ReadTable(plan->table).isErr()) {
        res.error = true;
        return res;
    }

    CatalogTable * table = catalog->ReadTable(plan->table).unwrap();

    if (plan->columns.size() == 0) {        
        for (int i = 0; i < plan->values.size(); i++) {
            if (!table->ValidateRow(plan->values.at(i))) {
                res.error = true;
                return res;
            }
        }

        for (int i = 0; i < plan->values.size(); i++) {
            Tuple t{plan->values.at(i), table};
            auto tuple_res = table->InsertTuple(t, catalog->buffer_manager);

            if (tuple_res.isErr()) {
                res.error = true;
                res.count = 0;
                return res;
            }
        }

        
    } else {
        throw; // Not supported yet
    }

    res.error = false;
    res.count = plan->values.size();
    return res;
}
