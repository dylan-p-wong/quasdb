
#include "tuple.h"
#include "../catalog/catalog.h"

Tuple::Tuple(const char * data_page, const int offset, const int size, const CatalogTable * catalog_table) : tuple_size{size}, data{new char[size]} {
    // Copy memory over
    memcpy(data, data_page + offset, tuple_size);
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
    } else if (datatype == DataType::Boolean) {
        bool value = *reinterpret_cast<bool*>(GetData() + column->GetColumnOffset());
        return std::make_unique<Data<bool>>(DataType::Boolean, value);
    } else if (datatype == DataType::Float) {
        float value = *reinterpret_cast<float*>(GetData() + column->GetColumnOffset());
        return std::make_unique<Data<float>>(DataType::Float, value);
    }

    throw Error{ErrorType::Internal, ""};
}

Tuple::Tuple(const std::vector<std::unique_ptr<AbstractData>> & values, const CatalogTable * catalog_table) {
    // Calculate Size
    tuple_size = catalog_table->GetLengthOfTuple();

    // Allocate memory
    data = new char[tuple_size];

    // Serialize into byte data
    for (int i = 0 ; i < catalog_table->GetNumberOfColumns(); i++) {
        CatalogColumn * catalog_column = catalog_table->GetColumn(i);

        if (values.at(i)->type == DataType::Integer) {
            memcpy(data + catalog_column->GetColumnOffset(), &dynamic_cast<Data<int>*>(values.at(i).get())->value, catalog_column->GetColumnSize());
        } else if (values.at(i)->type == DataType::Boolean) {
            memcpy(data + catalog_column->GetColumnOffset(), &dynamic_cast<Data<bool>*>(values.at(i).get())->value, catalog_column->GetColumnSize());
        } else if (values.at(i)->type == DataType::Float) {
            memcpy(data + catalog_column->GetColumnOffset(), &dynamic_cast<Data<float>*>(values.at(i).get())->value, catalog_column->GetColumnSize());
        } else {
            throw Error{ErrorType::Internal, ""};
        }
    }
}
