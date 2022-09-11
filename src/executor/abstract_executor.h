#pragma once

#include "../common/execution_output.h"
#include "../storage/catalog/catalog.h"

class AbstractExecutor {
public:
    AbstractExecutor();
    virtual ExecutionOutput Execute(Catalog * catalog) = 0;
    virtual ~AbstractExecutor() = 0;
};
