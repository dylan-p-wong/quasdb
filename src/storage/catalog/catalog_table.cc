#include <unordered_set>
#include <memory>

#include "catalog_table.h"

CatalogTable::CatalogTable(std::string name, std::vector<CatalogColumn*> columns, int tuple_size) : name{name}, columns{columns}, tuple_size{tuple_size} {}

Result<CatalogColumn*, Error> CatalogTable::GetColumn(const std::string & name) {
    for (CatalogColumn * column : columns) {
        if (column->GetColumnName() == name) {
            return Ok(column);
        }
    }

    return Err(Error{ErrorType::Internal, "Column with name does not exists in this table."});
}

std::vector<std::pair<std::string, std::string>> CatalogTable::GetReferences() {
    std::vector<std::pair<std::string, std::string>> res;

    for (CatalogColumn * column : columns) {
        if (column->GetReferences().has_value()) {
            res.push_back(column->GetReferences().value());
        }
    }

    return res;
}

bool CatalogTable::ValidateTable() {
    std::unordered_set<std::string> column_names;

    // Duplicate column names
    for (CatalogColumn * column : columns) {
        if (column_names.find(column->GetColumnName()) != column_names.end()) {
            return false;
        }
        column_names.insert(column->GetColumnName());
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

Result<void, Error> CatalogTable::InsertTuple(const InputTuple &tuple, BufferManager * buffer_manager) {
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

Result<OutputTuple*, Error> CatalogTable::GetTuple(const RID & rid, BufferManager * buffer_manager) {
    if (first_data_page_directory_page_id == -1) {
        return Err(Error{ErrorType::Internal, ""});
    }

    DirectoryPage * dp = reinterpret_cast<DirectoryPage*>(buffer_manager->GetPage(first_data_page_directory_page_id));
    return dp->GetTuple(rid, buffer_manager, this);
}

std::vector<OutputTuple*> CatalogTable::GetTuples(BufferManager * buffer_manager) {
    std::vector<OutputTuple*> res;

    int directory_page_id = first_data_page_directory_page_id;

    while (directory_page_id >= 0) {
        DirectoryPage * dp = reinterpret_cast<DirectoryPage*>(buffer_manager->GetPage(directory_page_id));

        for (int i = 0; i < dp->GetNumberOfDataPages(); i++) {
            if (dp->GetDataPagePageId(i) == -1) {
                continue;
            }
            TablePage * tp = reinterpret_cast<TablePage*>(buffer_manager->GetPage(dp->GetDataPagePageId(i)));

            for (int j = 0; j < tp->GetTupleCount(); j++) {
                auto tuple = tp->GetTuple(RID{tp->GetPageId(), j}, this);

                if (tuple.isOk()) {
                    res.push_back(tuple.unwrap());
                }
            }
        }

        directory_page_id = dp->GetNextDirectoryPageId();
    }

    return res;
}

Result<void, Error> CatalogTable::MarkDelete(const RID & rid, BufferManager * buffer_manager) {
    if (first_data_page_directory_page_id == -1) {
        return Err(Error{ErrorType::Internal, ""});
    }

    DirectoryPage * dp = reinterpret_cast<DirectoryPage*>(buffer_manager->GetPage(first_data_page_directory_page_id));
    return dp->MarkDelete(rid, buffer_manager, this);
}
