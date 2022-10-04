#include <gtest/gtest.h>

#include "db/quas.h"

class DBDiskTest : public ::testing::Test {
 protected:
  void SetUp() override {
      QuasDB db{"test"}; // So we delete
      db.Execute("CREATE TABLE course (cid integer not null, median float, name varchar(6))");
      db.Execute("CREATE TABLE enrollment (cid integer not null, sid integer not null)");
      db.Execute("CREATE TABLE student (sid integer not null, name varchar(36))");
  }
  void TearDown() override {
    QuasDB db{"test"};
    db.Delete();
  }
};

TEST_F(DBDiskTest, DBCatalogTest1) {
    QuasDB db{"test"};

    // EXPECT_EQ(db.GetFileSize(), 0);
    auto res = db.Execute("SELECT * FROM systeminfo");
    EXPECT_EQ(res.error, false);
    EXPECT_EQ(res.error_message, "");
    EXPECT_EQ(res.ToString(), "|course|18|2|3|\n|enrollment|8|5|2|\n|student|40|6|2|\n");
}

TEST_F(DBDiskTest, DBCatalogTest2) {
    QuasDB db{"test"};

    // EXPECT_EQ(db.GetFileSize(), 0);
    auto res = db.Execute("SELECT * FROM columninfo");
    EXPECT_EQ(res.error, false);
    EXPECT_EQ(res.error_message, "");
    EXPECT_EQ(res.ToString(), "|course|cid|1|0|0|4|\n|course|median|2|1|4|8|\n|course|name|4|1|12|6|\n|enrollment|cid|1|0|0|4|\n|enrollment|sid|1|0|4|4|\n|student|sid|1|0|0|4|\n|student|name|4|1|4|36|\n");
}
