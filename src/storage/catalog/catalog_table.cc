#include <unordered_set>
#include <memory>

#include "catalog_table.h"

CatalogTable::CatalogTable() {}

Result<CatalogColumn*, Error> CatalogTable::GetColumn(const std::string & name) {
    for (CatalogColumn * column : columns) {
        if (column->name == name) {
            return Ok(column);
        }
    }

    return Err(Error{ErrorType::Internal, "Column with name does not exists in this table."});
}

std::vector<std::pair<std::string, std::string>> CatalogTable::GetReferences() {
    std::vector<std::pair<std::string, std::string>> res;

    for (CatalogColumn * column : columns) {
        if (column->references.has_value()) {
            res.push_back(column->references.value());
        }
    }

    return res;
}

bool CatalogTable::ValidateTable() {
    std::unordered_set<std::string> column_names;

    // Duplicate column names
    for (CatalogColumn * column : columns) {
        if (column_names.find(column->name) != column_names.end()) {
            return false;
        }
        column_names.insert(column->name);
    }

    for (CatalogColumn * column : columns) {
        if (column->ValidateColumn() == false) {
            return false;
        }
    }

    return true;
}

bool CatalogTable::ValidateRow(const std::vector<std::unique_ptr<AbstractData>> & row) {

    if (row.size() != columns.size()) {
        return false;
    }

    for (int i = 0; i < row.size(); i++) {
        CatalogColumn * column = columns.at(i);
        
        if (!column->ValidateValue(row.at(i).get())) {
            return false;
        }
    }

    return true;
}

Result<void, Error> CatalogTable::InsertTuple(const Tuple &tuple, BufferManager * buffer_manager) {
    if (first_data_page_directory_page_id == -1) {
        first_data_page_directory_page_id = buffer_manager->NewPage()->GetPageId();
        DirectoryPage * p = reinterpret_cast<DirectoryPage*>(buffer_manager->GetPage(first_data_page_directory_page_id));
        p->Init();
    }

    DirectoryPage * p = reinterpret_cast<DirectoryPage*>(buffer_manager->GetPage(first_data_page_directory_page_id));

    while (p->InsertTuple(tuple, buffer_manager, this).isErr()) {
        if (p->GetNextDirectoryPageId() == -1) {
            DirectoryPage * new_page = reinterpret_cast<DirectoryPage*>(buffer_manager->NewPage());
            new_page->Init();
            new_page->SetNextDirectoryPageId(p->GetPageId());
            std::swap(p, new_page);
            delete new_page;
        } else {
            DirectoryPage * next_page = reinterpret_cast<DirectoryPage*>(buffer_manager->GetPage(p->GetNextDirectoryPageId()));
            std::swap(next_page, p);
            delete next_page;
        }
    }

    return Ok();
}
