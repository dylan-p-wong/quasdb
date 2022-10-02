#include <gtest/gtest.h>

#include "db/quas.h"

class DBDiskTest : public ::testing::Test {
 protected:
  void SetUp() override {
      QuasDB db{"DBCatalogTest1"}; // So we delete
      db.Execute("CREATE TABLE course (cid integer not null, median float, name varchar(6))");
      db.Execute("CREATE TABLE enrollment (cid integer not null, sid integer not null)");
      db.Execute("CREATE TABLE student (sid integer not null, name varchar(36))");
  }
  
};

TEST_F(DBDiskTest, DBCatalogTest1) {
    QuasDB db{"DBCatalogTest1"};

    auto res = db.Execute("CREATE TABLE course (cid integer not null, median float, name varchar(6))");
    EXPECT_EQ(res.error, false);
    res = db.Execute("CREATE TABLE enrollment (cid integer not null, sid integer not null)");
    EXPECT_EQ(res.error, false);
    res = db.Execute("CREATE TABLE student (sid integer not null, name varchar(36))");
    EXPECT_EQ(res.error, false);

    EXPECT_EQ(db.GetFileSize(), 0);
    res = db.Execute("SELECT * FROM systeminfo");
    EXPECT_EQ(res.error, false);
    EXPECT_EQ(res.error_message, "");
    EXPECT_EQ(res.ToString(), "|course|18|2|3|\n|enrollment|8|5|2|\n|student|40|6|2|\n");
    // EXPECT_EQ(res.rows.size(), 3);
    // EXPECT_EQ(res.rows.at(0).size(), 4);
    // EXPECT_EQ(res.rows.at(1).size(), 4);
    // EXPECT_EQ(res.rows.at(2).size(), 4);
}
