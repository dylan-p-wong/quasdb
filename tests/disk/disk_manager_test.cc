#include <gtest/gtest.h>

#include "storage/disk/disk_manager.h"
#include "storage/page/page.h"

TEST(DiskTest, DiskBasicTest1) {
    DiskManager * dm = new DiskManager{"test.db"};
    char data[4096]{};
    data[0] = 'a';
    dm->WritePage(0, data);
    EXPECT_EQ(dm->GetFileSize(), 4096);
    char read_data[4096]{};
    char * d = dm->ReadPage(0);
    memcpy(read_data, d, 4096);
    EXPECT_EQ(read_data[0], 'a');
}

TEST(DiskTest, DiskBasicTest2) {
    DiskManager * dm = new DiskManager{"test.db"};
    char data[4096]{};
    data[0] = 'a';
    data[1] = 'a';
    data[2] = 'a';
    dm->WritePage(3, data);
    EXPECT_EQ(dm->GetFileSize(), 4096 * 4);
    char read_data[4096]{};
    char * d = dm->ReadPage(3);
    memcpy(read_data, d, 4096);
    EXPECT_EQ(read_data[0], 'a');
    EXPECT_EQ(read_data[1], 'a');
    EXPECT_EQ(read_data[2], 'a');
}

TEST(DiskTest, DiskBasicTest3) {
    DiskManager * dm = new DiskManager{"test.db"};
    
    Page * page1 = new Page{0};
    dm->WritePage(page1->GetPageId(), page1->GetData());
    Page * read_page1 = new Page{dm->ReadPage(0)};
    EXPECT_EQ(read_page1->GetPageId(), 0);


    Page * page2 = new Page{1};
    dm->WritePage(page2->GetPageId(), page2->GetData());
    Page * read_page2 = new Page{dm->ReadPage(1)};
    EXPECT_EQ(read_page2->GetPageId(), 1);
}
