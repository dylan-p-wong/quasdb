#include <memory>

#include "../storage/buffer/buffer_manager.h"
#include "../storage/catalog/catalog.h"
#include "../planner/planner.h"
#include "../executor/executor.h"

class QuasDB {
    std::unique_ptr<BufferManager> buffer_manager;
    std::unique_ptr<Catalog> catalog;
    std::unique_ptr<Planner> planner;
    std::unique_ptr<Executor> executor;
public:
    QuasDB() : buffer_manager{std::make_unique<BufferManager>()}, catalog{std::make_unique<Catalog>(buffer_manager.get())}, planner{std::make_unique<Planner>(catalog.get())}, executor{std::make_unique<Executor>(catalog.get())} {}
    ExecutionOutput Execute(std::string query);
};
