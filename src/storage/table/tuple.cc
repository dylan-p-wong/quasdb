
#include "tuple.h"
#include "../catalog/catalog.h"

Tuple::Tuple(RID rid, const char * data_page, std::bitset<16> null_bit_map, const int offset, const int size, const CatalogTable * catalog_table) : null_bit_map{null_bit_map}, tuple_size{size}, data{new char[size]}, rid{rid} {
    // Copy memory over
    memcpy(data, data_page + offset, tuple_size);
}

std::unique_ptr<AbstractData> Tuple::GetValueAtColumnIndex(int index, const CatalogTable * catalog_table) const {
    if (index >= catalog_table->GetNumberOfColumns()) {
        return nullptr;
    }

    CatalogColumn * column = catalog_table->GetColumn(index);
    DataType datatype = column->GetColumnDataType();

    if (null_bit_map[index] == true) {
        return std::make_unique<Data<int>>(DataType::Null, 0);
    }

    if (datatype == DataType::Integer) {
        int value = *reinterpret_cast<int*>(GetData() + column->GetColumnOffset());
        return std::make_unique<Data<int>>(DataType::Integer, value);
    } else if (datatype == DataType::Boolean) {
        bool value = *reinterpret_cast<bool*>(GetData() + column->GetColumnOffset());
        return std::make_unique<Data<bool>>(DataType::Boolean, value);
    } else if (datatype == DataType::Float) {
        float value = *reinterpret_cast<float*>(GetData() + column->GetColumnOffset());
        return std::make_unique<Data<float>>(DataType::Float, value);
    } else if (datatype == DataType::Varchar) {
        char char_string[256]{};
        memcpy(char_string, GetData() + column->GetColumnOffset(), column->GetColumnSize());
        std::string res = char_string;
        return std::make_unique<Data<std::string>>(DataType::Varchar, res);
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
        } else if (values.at(i)->type == DataType::Null) {
            null_bit_map.set(i, true);
        } else if (values.at(i)->type == DataType::Varchar) {
            memcpy(data + catalog_column->GetColumnOffset(), &dynamic_cast<Data<std::string>*>(values.at(i).get())->value, catalog_column->GetColumnSize());
        } else {
            throw Error{ErrorType::Internal, ""};
        }
    }
}

std::vector<AbstractData*> Tuple::GetAsValues(const CatalogTable * catalog_table) {
    std::vector<AbstractData*> row;
    for (int i = 0; i < catalog_table->GetNumberOfColumns(); i++) {
        row.emplace_back(GetValueAtColumnIndex(i, catalog_table).release());
    }
    return row;
}
