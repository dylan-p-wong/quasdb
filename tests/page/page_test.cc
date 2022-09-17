#include <gtest/gtest.h>

#include "storage/page/directory_page.h"
#include "storage/page/table_page.h"
#include "storage/catalog/catalog.h"

TEST(PageTest, DirectoryPageBasicTest1) {
    DirectoryPage * dp = reinterpret_cast<DirectoryPage*>(new Page{0});
    dp->Init();
    EXPECT_EQ(dp->GetPageId(), 0);
    EXPECT_EQ(dp->GetPreviousDirectoryPageId(), -1);
    EXPECT_EQ(dp->GetNextDirectoryPageId(), -1);

    EXPECT_EQ(dp->GetDataPagePageId(0), -1);
    EXPECT_EQ(dp->GetDataPagePageId(1), -1);
    EXPECT_EQ(dp->GetDataPageFreeSpace(0), 0);
    EXPECT_EQ(dp->GetDataPageFreeSpace(1), 0);

    dp->SetPreviousDirectoryPageId(5);
    EXPECT_EQ(dp->GetPreviousDirectoryPageId(), 5);

    dp->SetNextDirectoryPageId(4);
    EXPECT_EQ(dp->GetNextDirectoryPageId(), 4);

    dp->SetNumberOfDataPages(6);
    EXPECT_EQ(dp->GetNumberOfDataPages(), 6);

    EXPECT_EQ(dp->GetMaxNumberOfDataPages(), 496);
}

TEST(PageTest, TablePageBasicTest1) {
    TablePage * tp = reinterpret_cast<TablePage*>(new Page{1});
    tp->Init();
    EXPECT_EQ(tp->GetPageId(), 1);
    EXPECT_EQ(tp->GetTupleCount(), 0);
    EXPECT_EQ(tp->GetFreeSpacePointer(), 4096);

    EXPECT_EQ(tp->GetFreeSpace(), 4096 - 12);

    tp->SetTupleCount(5);
    EXPECT_EQ(tp->GetTupleCount(), 5);

    tp->SetFreeSpacePointer(4);
    EXPECT_EQ(tp->GetFreeSpacePointer(), 4);
}
