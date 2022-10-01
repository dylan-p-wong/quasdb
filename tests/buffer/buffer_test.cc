#include <gtest/gtest.h>
#include <memory>

#include "storage/buffer/buffer_manager.h"

TEST(BufferTest, BufferBasicTest1) {
    std::unique_ptr<BufferManager> bm = std::make_unique<BufferManager>();

    Page * page0 = bm->NewPage();

    Page * page1 = bm->NewPage();
    Page * page2 = bm->NewPage();
    Page * page3 = bm->NewPage();

    EXPECT_EQ(page0->GetPageId(), 0);
    EXPECT_EQ(page1->GetPageId(), 1);
    EXPECT_EQ(page2->GetPageId(), 2);
    EXPECT_EQ(page3->GetPageId(), 3);

    EXPECT_EQ(page0, bm->GetPage(0));
    EXPECT_EQ(page1, bm->GetPage(1));
    EXPECT_EQ(page2, bm->GetPage(2));
    EXPECT_EQ(page3, bm->GetPage(3));
}

// TEST(BufferTest, BufferBasicTest2) {
//     std::unique_ptr<BufferManager> bm = std::make_unique<BufferManager>();

//     for (int i = 0; i < 64; i++) {
//         bm->NewPage();
//     }

//     EXPECT_EQ(bm->GetPage(64), nullptr);
//     EXPECT_NE(bm->GetPage(63), nullptr);

//     EXPECT_EQ(bm->empty_buffer_slots_list.size(), 0);

//     Page * new_page = bm->NewPage();

//     EXPECT_NE(bm->GetPage(64), nullptr);
//     EXPECT_EQ(new_page->GetPageId(), 64);
//     EXPECT_EQ(bm->GetPage(64)->GetPageId(), 64);
//     EXPECT_EQ(bm->GetPage(64), new_page);

//     EXPECT_EQ(bm->GetPage(0), nullptr);
//     EXPECT_NE(bm->GetPage(63), nullptr);
// }
