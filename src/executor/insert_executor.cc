#include "insert_executor.h"

InsertExecutor::InsertExecutor(const InsertPlan * plan) : AbstractExecutor{}, plan{plan} {}

std::vector<std::vector<AbstractData*>> InsertExecutor::Execute(Catalog * catalog) {
    if (catalog->ReadTable(plan->table).isErr()) {
        throw Error{ErrorType::Internal, ""};
    }

    CatalogTable * table = catalog->ReadTable(plan->table).unwrap();

    if (plan->columns.size() == 0) {
        for (int i = 0; i < plan->values.size(); i++) {
            if (!table->ValidateRow(plan->values.at(i))) {
                throw Error{ErrorType::Internal, ""};
            }
        }

        for (int i = 0; i < plan->values.size(); i++) {
            Tuple t{plan->values.at(i), table};
            auto tuple_res = table->InsertTuple(t, catalog->GetCatalogBufferManager());

            if (tuple_res.isErr()) {
                throw tuple_res.unwrapErr();
            }
        }
        
    } else {
        throw Error{ErrorType::Internal, ""}; // Not supported yet
    }
    std::vector<std::vector<AbstractData*>> res;
    return res;
}
