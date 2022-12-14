#include <memory>

#include "../storage/disk/disk_manager.h"
#include "../storage/buffer/buffer_manager.h"
#include "../storage/catalog/catalog.h"
#include "../planner/planner.h"
#include "../executor/executor.h"

class QuasDB {
    std::string name;
    std::unique_ptr<DiskManager> disk_manager;
    std::unique_ptr<BufferManager> buffer_manager;
    std::unique_ptr<Catalog> catalog;
    std::unique_ptr<Planner> planner;
    std::unique_ptr<Executor> executor;
public:
    QuasDB(std::string name) : name{name}, disk_manager{std::make_unique<DiskManager>(name + ".db")}, buffer_manager{std::make_unique<BufferManager>(disk_manager.get())}, catalog{std::make_unique<Catalog>(buffer_manager.get())}, planner{std::make_unique<Planner>(catalog.get())}, executor{std::make_unique<Executor>(catalog.get())} {}
    ExecutionOutput Execute(std::string query);
    int GetFileSize() {
        return disk_manager->GetFileSize();
    }
    void Delete() {
        // disk_manager->DeleteFile();
    }
};
