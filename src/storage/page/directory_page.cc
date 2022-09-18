#include "directory_page.h"

void DirectoryPage::Init() {
    SetPreviousDirectoryPageId(-1);
    SetNextDirectoryPageId(-1);
    SetNumberOfDataPages(0);

    for (int i = 0; i < GetMaxNumberOfDataPages(); i++) {
        SetDataPagePageId(i, -1);
        SetDataPageFreeSpace(i, 0);
    }
}

Result<void, Error> DirectoryPage::InsertTuple(const Tuple &tuple, BufferManager * buffer_manager, CatalogTable * catalog_table) {
    // try to find space in current directory page
    for (int i = 0; i < GetMaxNumberOfDataPages(); i++) {
        
        if (GetDataPagePageId(i) == -1) {
            TablePage * tp = reinterpret_cast<TablePage*>(buffer_manager->NewPage());
            tp->Init();
            SetDataPagePageId(i, tp->GetPageId());
            SetDataPageFreeSpace(i, tp->GetFreeSpace());
        }

        TablePage * tp = reinterpret_cast<TablePage*>(buffer_manager->GetPage(GetDataPagePageId(i)));

        if (tp->GetFreeSpace() > tuple.tuple_size) {
            Result<void, Error> res = tp->InsertTuple(tuple, catalog_table); // still need to handle even though should not error
            SetDataPageFreeSpace(i, tp->GetFreeSpace());
            return res;
        }
    }

    // no free space we must create or go to the next directory or create a page
    if (GetNextDirectoryPageId() == -1) {
        DirectoryPage * dp = reinterpret_cast<DirectoryPage*>(buffer_manager->NewPage());
        dp->Init();
        SetNextDirectoryPageId(dp->GetPageId());
        dp->SetPreviousDirectoryPageId(GetPageId());
    }

    DirectoryPage * dp = reinterpret_cast<DirectoryPage*>(buffer_manager->GetPage(GetNextDirectoryPageId()));

    return dp->InsertTuple(tuple, buffer_manager, catalog_table);
}

Result<Tuple*, Error> DirectoryPage::GetTuple(const RID &rid, BufferManager * buffer_manager, CatalogTable * catalog_table) {
    for (int i = 0; i < GetMaxNumberOfDataPages(); i++) {
        if (GetDataPagePageId(i) == -1) {
            break;
        }

        if (GetDataPagePageId(i) == rid.page_number) {
            TablePage * tp = reinterpret_cast<TablePage*>(buffer_manager->GetPage(rid.page_number));
            return tp->GetTuple(rid, catalog_table);
        }
    }
    if (GetNextDirectoryPageId() == -1) {
        return Err(Error{ErrorType::Internal, ""});
    }
    DirectoryPage * dp = reinterpret_cast<DirectoryPage*>(buffer_manager->GetPage(GetNextDirectoryPageId()));
    return dp->GetTuple(rid, buffer_manager, catalog_table);
}
