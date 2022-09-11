#pragma once

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
};
