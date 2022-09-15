#include <gtest/gtest.h>

#include "storage/page/directory_page.h"
#include "storage/page/table_page.h"

TEST(PageTest, DirectoryPageBasicTest1) {
    DirectoryPage * dp = new DirectoryPage{0};
    EXPECT_EQ(dp->GetPageId(), 0);

    dp->SetPreviousDirectoryPageId(5);
    EXPECT_EQ(dp->GetPreviousDirectoryPageId(), 5);

    dp->SetNextDirectoryPageId(4);
    EXPECT_EQ(dp->GetNextDirectoryPageId(), 4);

    dp->SetNumberOfDataPages(6);
    EXPECT_EQ(dp->GetNumberOfDataPages(), 6);

    EXPECT_EQ(dp->GetMaxNumberOfDataPages(), 496);
}

TEST(PageTest, TablePageBasicTest1) {
    TablePage * tp = new TablePage{1};
    EXPECT_EQ(tp->GetPageId(), 1);

    tp->SetTupleCount(5);
    EXPECT_EQ(tp->GetTupleCount(), 5);

    tp->SetFreeSpacePointer(4);
    EXPECT_EQ(tp->GetFreeSpacePointer(), 4);

    tp->SetTupleSize(88);
    EXPECT_EQ(tp->GetTupleSize(), 88);
}
