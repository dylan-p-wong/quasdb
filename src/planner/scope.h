#pragma once

#include <unordered_map>
#include <string>

class Scope {
public:
    int number_of_columns;
    std::unordered_map<std::string, int> table_field_name_to_column_index;
    int GetFieldIndex(std::string field) const {
        if (table_field_name_to_column_index.find(field) == table_field_name_to_column_index.end()) {
            return -1;
        }

        return table_field_name_to_column_index.at(field);
    }

    bool AddFieldToScope(std::string field, int index) {
        table_field_name_to_column_index[field] = index;
    }

    int GetNumberOfColumns() const {
        return number_of_columns;
    }

    // left: 0, 1, 2
    // right: 0, 1
    // left: 0, 1, 2
    // right: 3, 4
    Scope Merge(const Scope & other) {
        Scope s{this->GetNumberOfColumns() + other.GetNumberOfColumns()};

        for (auto & it: this->table_field_name_to_column_index) {
            s.AddFieldToScope(it.first, it.second);
        }

        for (auto & it: other.table_field_name_to_column_index) {
            if (GetFieldIndex(it.first) == -1) {
                s.AddFieldToScope(it.first, it.second + this->GetNumberOfColumns());
            } else {
                throw Error{ErrorType::Internal, it.first};
            }
        }

        return s;
    }
    Scope(int number_of_columns) : number_of_columns{number_of_columns} {}
};
