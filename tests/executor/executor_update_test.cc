#include <gtest/gtest.h>

#include "storage/catalog/catalog.h"
#include "planner/planner.h"
#include "parser/parser.h"
#include "executor/executor.h"

class TableTest : public ::testing::Test {
 protected:
  void SetUp() override {
    disk_manager = nullptr;
    buffer_manager = new BufferManager{disk_manager};
    catalog = new Catalog{buffer_manager};
    planner = new Planner{catalog};
    executor = new Executor{catalog};

    Parser parser1{"CREATE table test1 (id integer, name varchar(4))"};
    executor->Execute(planner->CreatePlan(parser1.ParseStatement().unwrap()).get());

    Parser parser2{"INSERT INTO test1 VALUES (1, 'test'), (2, 'test'), (3, 'test'), (4, 'test'), (5, 'test'), (6, 'test'), (7, 'test'), (8, 'test'), (9, 'test'), (10, 'test')"};
    executor->Execute(planner->CreatePlan(parser2.ParseStatement().unwrap()).get());
  }

  BufferManager * buffer_manager;
  Catalog * catalog;
  Planner * planner;
  Executor * executor;
  DiskManager * disk_manager;
};

TEST_F(TableTest, SelectValid) {
    Parser parser{"SELECT * from test1"};
    ExecutionOutput res = executor->Execute(planner->CreatePlan(parser.ParseStatement().unwrap()).get());
    EXPECT_EQ(res.rows.size(), 10);
}

TEST_F(TableTest, UpdateTest1) {
    Parser parser1{"UPDATE test1 SET id=99"};
    ExecutionOutput res1 = executor->Execute(planner->CreatePlan(parser1.ParseStatement().unwrap()).get());
    EXPECT_EQ(res1.error, false);

    Parser parser{"SELECT * from test1 WHERE test1.id=99"};
    ExecutionOutput res = executor->Execute(planner->CreatePlan(parser.ParseStatement().unwrap()).get());
    EXPECT_EQ(res.error, false);
    EXPECT_EQ(res.rows.size(), 10);

    Parser parser2{"SELECT * from test1"};
    ExecutionOutput res2 = executor->Execute(planner->CreatePlan(parser2.ParseStatement().unwrap()).get());
    EXPECT_EQ(res2.rows.size(), 10);
}

TEST_F(TableTest, UpdateTest2) {
    Parser parser1{"UPDATE test1 SET id=99 where test1.id=8 or test1.id=9"};
    ExecutionOutput res1 = executor->Execute(planner->CreatePlan(parser1.ParseStatement().unwrap()).get());
    EXPECT_EQ(res1.error, false);

    Parser parser{"SELECT * from test1 WHERE test1.id=99"};
    ExecutionOutput res = executor->Execute(planner->CreatePlan(parser.ParseStatement().unwrap()).get());
    EXPECT_EQ(res.error, false);
    EXPECT_EQ(res.rows.size(), 2);

    Parser parser2{"SELECT * from test1"};
    ExecutionOutput res2 = executor->Execute(planner->CreatePlan(parser2.ParseStatement().unwrap()).get());
    EXPECT_EQ(res2.rows.size(), 10);
}

TEST_F(TableTest, UpdateTest3) {
    Parser parser1{"UPDATE test1 SET id=99, name='nas' where test1.id=8 or test1.id=9"};
    ExecutionOutput res1 = executor->Execute(planner->CreatePlan(parser1.ParseStatement().unwrap()).get());
    EXPECT_EQ(res1.error, false);

    Parser parser{"SELECT * from test1 WHERE test1.id=99"};
    ExecutionOutput res = executor->Execute(planner->CreatePlan(parser.ParseStatement().unwrap()).get());
    EXPECT_EQ(res.error, false);
    EXPECT_EQ(res.rows.size(), 2);

    Parser parser3{"SELECT * from test1 where test1.id=8 or test1.id=9"};
    ExecutionOutput res3 = executor->Execute(planner->CreatePlan(parser3.ParseStatement().unwrap()).get());
    EXPECT_EQ(res3.error, false);
    EXPECT_EQ(res3.rows.size(), 0);

    Parser parser2{"SELECT * from test1"};
    ExecutionOutput res2 = executor->Execute(planner->CreatePlan(parser2.ParseStatement().unwrap()).get());
    EXPECT_EQ(res2.rows.size(), 10);
}
