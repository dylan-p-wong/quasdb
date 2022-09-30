#include <gtest/gtest.h>

#include "db/quas.h"

class DBTest : public ::testing::Test {
 protected:
  void SetUp() override {}
  QuasDB db;
};

TEST_F(DBTest, DBTest1) {
  auto res = db.Execute("SELECT * FROM test");
  EXPECT_EQ(res.error, true);
  res = db.Execute("CREATE TABLE course (cid integer not null, median float, name varchar(6))");
  EXPECT_EQ(res.error, false);
  res = db.Execute("CREATE TABLE enrollment (cid integer not null, sid integer not null)");
  EXPECT_EQ(res.error, false);
  res = db.Execute("CREATE TABLE student (sid integer not null, name varchar(36))");
  EXPECT_EQ(res.error, false);

  res = db.Execute("INSERT into course VALUES (-3, 23.6, 'MATH'), (0, 67.9, 'CS'), (1, 98.7, 'CS'), (7, 89.9999, 'MATH'), (-2, 23.6, 'MATH'), (22, 98.7, 'CS'), (-1, 89.9999, 'MATH')");
  EXPECT_EQ(res.error, false);
  res = db.Execute("INSERT into enrollment VALUES (0, 1), (0, 1), (0, 1), (1, 2), (2, 4), (-1, 2), (0, 1), (1, 1), (0, -9)");
  EXPECT_EQ(res.error, false);
  res = db.Execute("INSERT into student VALUES (0, 'Frank'), (1, 'Robert'), (2, 'Kanye'), (3, 'Dylan'), (4, 'Nas'), (5, 'DOOM'), (6, 'GZA'), (7, 'RZA'), (8, 'Drake'), (9, 'Billy Woods')");
  EXPECT_EQ(res.error, false);

  res = db.Execute("SELECT * from course where course.name='MATH'");
  EXPECT_EQ(res.error, false);
  EXPECT_EQ(res.rows.size(), 4);

  res = db.Execute("SELECT * from course");
  EXPECT_EQ(res.error, false);
  EXPECT_EQ(res.rows.size(), 7);

  res = db.Execute("SELECT * from enrollment");
  EXPECT_EQ(res.error, false);
  EXPECT_EQ(res.rows.size(), 9);

  res = db.Execute("SELECT * from student");
  EXPECT_EQ(res.error, false);
  EXPECT_EQ(res.rows.size(), 10);

  res = db.Execute("SELECT * from course JOIN enrollment ON course.cid=enrollment.cid");
  EXPECT_EQ(res.error, false);
  EXPECT_EQ(res.rows.size(), 8);

  res = db.Execute("SELECT * from course JOIN enrollment ON course.cid=enrollment.cid JOIN student on enrollment.sid = student.sid");
  EXPECT_EQ(res.error, false);
  EXPECT_EQ(res.rows.size(), 7);
  EXPECT_EQ(res.rows.at(0).size(), 7);
  EXPECT_EQ(res.rows.at(1).size(), 7);
  EXPECT_EQ(res.rows.at(2).size(), 7);
  EXPECT_EQ(res.rows.at(3).size(), 7);
  EXPECT_EQ(res.rows.at(4).size(), 7);
  EXPECT_EQ(res.rows.at(5).size(), 7);
  EXPECT_EQ(res.rows.at(6).size(), 7);

  res = db.Execute("SELECT * from course JOIN enrollment ON course.cid=enrollment.cid JOIN student on enrollment.sid = student.sid ORDER BY student.sid");
  EXPECT_EQ(res.error, false);
  EXPECT_EQ(res.rows.size(), 7);
  EXPECT_EQ(res.rows.at(0).size(), 7);
  EXPECT_EQ(res.rows.at(1).size(), 7);
  EXPECT_EQ(res.rows.at(2).size(), 7);
  EXPECT_EQ(res.rows.at(3).size(), 7);
  EXPECT_EQ(res.rows.at(4).size(), 7);
  EXPECT_EQ(res.rows.at(5).size(), 7);
  EXPECT_EQ(res.rows.at(6).size(), 7);
}

TEST_F(DBTest, DBTest2) {
  auto res = db.Execute("CREATE TABLE test (x integer)");
  EXPECT_EQ(res.error, false);

  res = db.Execute("SELECT * FROM test");
  EXPECT_EQ(res.error, false);

  res = db.Execute("  insert into test values (1),(2)");
  EXPECT_EQ(res.error, false);

  res = db.Execute("DELETE FROM test");
  EXPECT_EQ(res.error, false);
}
