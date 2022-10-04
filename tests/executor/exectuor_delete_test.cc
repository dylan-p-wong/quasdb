#include <gtest/gtest.h>

#include "storage/catalog/catalog.h"
#include "planner/planner.h"
#include "parser/parser.h"
#include "executor/executor.h"

class TableTest : public ::testing::Test {
 protected:
  void SetUp() override {
    buffer_manager = new BufferManager{disk_manager};
    catalog = new Catalog{buffer_manager};
    planner = new Planner{catalog};
    executor = new Executor{catalog};

    Parser parser1{"CREATE table test1 (id integer, name varchar(91))"};
    executor->Execute(planner->CreatePlan(parser1.ParseStatement().unwrap()).get());

    Parser parser2{"INSERT INTO test1 VALUES (1, 'test'), (2, 'test'), (3, 'test'), (4, 'test'), (5, 'test'), (6, 'test'), (7, 'test'), (8, 'test'), (9, 'test'), (10, 'test')"};
    executor->Execute(planner->CreatePlan(parser2.ParseStatement().unwrap()).get());
  }

  BufferManager * buffer_manager;
  Catalog * catalog;
  Planner * planner;
  Executor * executor;
  DiskManager * disk_manager = nullptr;
};

TEST_F(TableTest, SelectValid) {
    Parser parser{"SELECT * from test1"};
    ExecutionOutput res = executor->Execute(planner->CreatePlan(parser.ParseStatement().unwrap()).get());
    EXPECT_EQ(res.rows.size(), 10);
}

TEST_F(TableTest, DeleteTest1) {
    Parser parser1{"DELETE FROM test1"};
    ExecutionOutput res1 = executor->Execute(planner->CreatePlan(parser1.ParseStatement().unwrap()).get());
    
    Parser parser{"SELECT * from test1"};
    ExecutionOutput res = executor->Execute(planner->CreatePlan(parser.ParseStatement().unwrap()).get());
    EXPECT_EQ(res.rows.size(), 0);
}

TEST_F(TableTest, DeleteTest2) {
    Parser parser1{"DELETE FROM test1 where test1.id=5 or test1.id=9"};
    ExecutionOutput res1 = executor->Execute(planner->CreatePlan(parser1.ParseStatement().unwrap()).get());
    
    Parser parser{"SELECT * from test1"};
    ExecutionOutput res = executor->Execute(planner->CreatePlan(parser.ParseStatement().unwrap()).get());
    EXPECT_EQ(res.rows.size(), 8);
}

TEST_F(TableTest, DeleteTest3) {
    Parser parser1{"DELETE FROM test1 where test1.id=11 and test1.id=9"};
    ExecutionOutput res1 = executor->Execute(planner->CreatePlan(parser1.ParseStatement().unwrap()).get());
    
    Parser parser{"SELECT * from test1"};
    ExecutionOutput res = executor->Execute(planner->CreatePlan(parser.ParseStatement().unwrap()).get());
    EXPECT_EQ(res.rows.size(), 10);
}
