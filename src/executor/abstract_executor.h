#pragma once

#include "../common/execution_output.h"
#include "../storage/catalog/catalog.h"

class AbstractExecutor {
public:
    AbstractExecutor();
    virtual std::vector<std::vector<AbstractData*>> Execute(Catalog * catalog) = 0;
    virtual ~AbstractExecutor() = 0;
};
