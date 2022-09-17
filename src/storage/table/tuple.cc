
#include "tuple.h"
#include "../catalog/catalog.h"

Tuple::Tuple(const CatalogTable * catalog_table) {
    // Calculate Size
    tuple_size = catalog_table->GetLengthOfTuple();

    // Allocate memory
    data = new char[tuple_size];
}

std::unique_ptr<AbstractData> Tuple::GetValueAtColumnIndex(int index, const CatalogTable * catalog_table) {
    if (index >= catalog_table->GetNumberOfColumns()) {
        return nullptr;
    }

    CatalogColumn * column = catalog_table->columns.at(index);
    DataType datatype = column->GetColumnDataType();

    if (datatype == DataType::Integer) {
        int value = *reinterpret_cast<int*>(GetData() + column->GetColumnOffset());
        return std::make_unique<Data<int>>(DataType::Integer, value);
    }

    throw;
}

Tuple::Tuple(const std::vector<std::unique_ptr<AbstractData>> & values, const CatalogTable * catalog_table) {
    // Calculate Size
    tuple_size = catalog_table->GetLengthOfTuple();

    // Allocate memory
    data = new char[tuple_size];

    // Serialize into byte data
    for (int i = 0 ; i < catalog_table->GetNumberOfColumns(); i++) {
        CatalogColumn * catalog_column = catalog_table->GetColumn(i);
        memcpy(data + catalog_column->GetColumnOffset(), &dynamic_cast<Data<int>*>(values.at(i).get())->value, catalog_column->GetColumnSize());
    }
}
