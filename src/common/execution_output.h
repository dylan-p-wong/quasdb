#pragma once

#include <string>
#include <vector>

#include "shared.h"
#include "error.h"

enum class OutputType
{
    // Begin,
    // Commit,
    // Rollback,
    // Explain,
    CreateTable,
    DropTable,
    Delete,
    Insert,
    Update,
    Select
};

class ExecutionOutput {
public:
    OutputType type;
    int count;
    std::vector<std::string> columns;
    std::vector<std::vector<AbstractData*>> rows;

    bool error;
    ErrorType error_type;
    std::string error_message;

    std::string ToString() {
        std::string res;
        for (std::vector<AbstractData*> row : rows) {
            res += "|";
            for (AbstractData * data : row) {
                if (data->type == DataType::Integer) {
                    res += std::to_string(dynamic_cast<Data<int>*>(data)->value);
                } else if (data->type == DataType::Boolean) {
                    res += std::to_string(dynamic_cast<Data<bool>*>(data)->value);
                } else if (data->type == DataType::Float) {
                    res += std::to_string(dynamic_cast<Data<float>*>(data)->value);
                } else if (data->type == DataType::Null) {
                    res += "NULL";
                } else if (data->type == DataType::Varchar) {
                    res += dynamic_cast<Data<std::string>*>(data)->value;
                } else {
                    throw Error{ErrorType::Internal, "Error converting result to string."};
                }
                res += "|";
            }

            res += "\n";
        }
        return res;
    }
};
