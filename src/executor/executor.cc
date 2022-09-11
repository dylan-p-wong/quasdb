#include "executor.h"

Executor::Executor(Catalog * catalog) : catalog{catalog} {}

ExecutionOutput Executor::Execute(const PlanNode * plan){
    std::unique_ptr<AbstractExecutor> executor = ExecutorFactory::CreateExecutor(plan);
    return executor.get()->Execute(catalog);
}
