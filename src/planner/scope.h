#pragma once

#include <unordered_map>
#include <string>

class Scope {
public:
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
};
