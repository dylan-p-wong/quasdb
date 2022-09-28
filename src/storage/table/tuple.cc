
#include "tuple.h"
#include "../catalog/catalog.h"

Tuple::Tuple(std::bitset<16> null_bit_map, char * data, int tuple_size) : null_bit_map{null_bit_map}, data{data}, tuple_size{tuple_size} {}
Tuple::Tuple(char * data, int tuple_size) : data{data}, tuple_size{tuple_size} {}

OutputTuple::OutputTuple(RID rid, const char * data_page, std::bitset<16> null_bit_map, const int offset, const int tuple_size) : Tuple{null_bit_map, new char[tuple_size], tuple_size}, rid{rid} {
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

std::vector<AbstractData*> Tuple::GetAsValues(const CatalogTable * catalog_table) {
    std::vector<AbstractData*> row;
    for (int i = 0; i < catalog_table->GetNumberOfColumns(); i++) {
        row.emplace_back(GetValueAtColumnIndex(i, catalog_table).release());
    }
    return row;
}

InputTuple::InputTuple(const std::vector<std::unique_ptr<AbstractData>> & values, const CatalogTable * catalog_table) : Tuple{new char[catalog_table->GetLengthOfTuple()], catalog_table->GetLengthOfTuple()} {
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


void Tuple::Update(std::string column_name, AbstractData * a_data, const CatalogTable * catalog_table) {
    int i = catalog_table->GetColumnIndexByName(column_name);
    CatalogColumn * catalog_column = catalog_table->GetColumn(i);

    if (a_data->type == DataType::Integer) {
        memcpy(data + catalog_column->GetColumnOffset(), &dynamic_cast<Data<int>*>(a_data)->value, catalog_column->GetColumnSize());
    } else if (a_data->type == DataType::Boolean) {
        memcpy(data + catalog_column->GetColumnOffset(), &dynamic_cast<Data<bool>*>(a_data)->value, catalog_column->GetColumnSize());
    } else if (a_data->type == DataType::Float) {
        memcpy(data + catalog_column->GetColumnOffset(), &dynamic_cast<Data<float>*>(a_data)->value, catalog_column->GetColumnSize());
    } else if (a_data->type == DataType::Null) {
        null_bit_map.set(i, true);
    } else if (a_data->type == DataType::Varchar) {
        memcpy(data + catalog_column->GetColumnOffset(), &dynamic_cast<Data<std::string>*>(a_data)->value, catalog_column->GetColumnSize());
    } else {
        throw Error{ErrorType::Internal, ""};
    }
}
