#include "executor.h"

Executor::Executor(Catalog * catalog) : catalog{catalog} {}

ExecutionOutput Executor::Execute(const PlanNode * plan){
    std::unique_ptr<AbstractExecutor> executor = ExecutorFactory::CreateExecutor(plan);
    ExecutionOutput res;
    res.error = false;
    try {
        res.rows = executor.get()->Execute(catalog);
    } catch (Error & e) {
        res.error_message = e.message;
        res.error = true;
    }
    return res;
}
