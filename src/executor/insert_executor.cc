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
            InputTuple t{plan->values.at(i), table};
            auto tuple_res = table->InsertTuple(t, catalog->GetCatalogBufferManager());

            if (tuple_res.isErr()) {
                throw tuple_res.unwrapErr();
            }
        }
        
    } else {
        for (int i = 0; i < plan->values.size(); i++) {            
            if (plan->values.at(i).size() != plan->columns.size()) {
                throw Error{ErrorType::Internal, "Invalid size of value."};
            }

            std::vector<std::unique_ptr<AbstractData>> updated_value;

            for (int j = 0; j < table->GetNumberOfColumns(); j++) {
                CatalogColumn * c = table->GetColumn(j);
                
                int found = -1;
                for (int k = 0; k < plan->columns.size(); k++) {
                    if (plan->columns.at(k) == c->GetColumnName()) {
                        found = k;
                    }
                }
                
                if (found >= 0) {
                    updated_value.emplace_back(plan->values.at(i).at(found)->Copy());
                } else {
                    updated_value.emplace_back(std::make_unique<Data<int>>(DataType::Null, 0));
                }
            }

            // validate
            if (!table->ValidateRow(updated_value)) {
                throw Error{ErrorType::Internal, ""};
            }

            // insert
            InputTuple t{updated_value, table};
            auto tuple_res = table->InsertTuple(t, catalog->GetCatalogBufferManager());

            if (tuple_res.isErr()) {
                throw tuple_res.unwrapErr();
            }
        }
    }
    std::vector<std::vector<AbstractData*>> res;
    return res;
}
