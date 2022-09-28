#include <gtest/gtest.h>

#include "storage/catalog/catalog.h"
#include "planner/planner.h"
#include "parser/parser.h"
#include "executor/executor.h"

class TableTest : public ::testing::Test {
 protected:
  void SetUp() override {
    buffer_manager = new BufferManager{};
    catalog = new Catalog{buffer_manager};
    planner = new Planner{catalog};
    executor = new Executor{catalog};

    Parser parser1{"CREATE table test1 (id integer, name varchar(4))"};
    executor->Execute(planner->CreatePlan(parser1.ParseStatement().unwrap()).get());

    Parser parser2{"INSERT INTO test1 VALUES (1, 'aaab'), (2, 'aaaa'), (3, 'aaaa'), (4, 'bab')"};
    // Parser parser2{"INSERT INTO test1 VALUES (1, 'aaab'), (2, 'aaaa'), (3, 'aaaa'), (4, 'bab'), (5, 'casd'), (6, 'aaaa'), (7, 'abab'), (8, 'abbb'), (9, 'aaaa'), (10, 'aaaa')"};
    executor->Execute(planner->CreatePlan(parser2.ParseStatement().unwrap()).get());
  }

  BufferManager * buffer_manager;
  Catalog * catalog;
  Planner * planner;
  Executor * executor;
};

TEST_F(TableTest, SelectValid1) {
    Parser parser{"SELECT * from test1 ORDER BY test1.id"};
    ExecutionOutput res = executor->Execute(planner->CreatePlan(parser.ParseStatement().unwrap()).get());
    EXPECT_EQ(res.rows.size(), 4);
    EXPECT_EQ(res.error, false);
    EXPECT_EQ(res.error_message, "");
    EXPECT_EQ(res.ToString(), "|1|aaab|\n|2|aaaa|\n|3|aaaa|\n|4|bab|\n");
}

TEST_F(TableTest, SelectValid2) {
    Parser parser{"SELECT * from test1 ORDER BY test1.name, test1.id"};
    ExecutionOutput res = executor->Execute(planner->CreatePlan(parser.ParseStatement().unwrap()).get());
    EXPECT_EQ(res.rows.size(), 4);
    EXPECT_EQ(res.error, false);
    EXPECT_EQ(res.error_message, "");
    EXPECT_EQ(res.ToString(), "|2|aaaa|\n|3|aaaa|\n|1|aaab|\n|4|bab|\n");
}

TEST_F(TableTest, SelectValid3) {
    Parser parser{"SELECT * from test1 ORDER BY test1.name DESC, test1.id DESC"};
    ExecutionOutput res = executor->Execute(planner->CreatePlan(parser.ParseStatement().unwrap()).get());
    EXPECT_EQ(res.rows.size(), 4);
    EXPECT_EQ(res.error, false);
    EXPECT_EQ(res.error_message, "");
    EXPECT_EQ(res.ToString(), "|4|bab|\n|1|aaab|\n|3|aaaa|\n|2|aaaa|\n");
}

TEST_F(TableTest, SelectValid4) {
    Parser parser{"SELECT * from test1 ORDER BY test1.name ASC, test1.id DESC"};
    ExecutionOutput res = executor->Execute(planner->CreatePlan(parser.ParseStatement().unwrap()).get());
    EXPECT_EQ(res.rows.size(), 4);
    EXPECT_EQ(res.error, false);
    EXPECT_EQ(res.error_message, "");
    EXPECT_EQ(res.ToString(), "|3|aaaa|\n|2|aaaa|\n|1|aaab|\n|4|bab|\n");
}