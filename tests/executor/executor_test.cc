#include <gtest/gtest.h>

#include "storage/catalog/catalog.h"
#include "planner/planner.h"
#include "parser/parser.h"
#include "planner/plans/create_table_plan.h"
#include "planner/plans/drop_table_plan.h"

#include "executor/executor.h"

TEST(ExecutorTest, ExecutorCreateTableTest1) {
  Catalog * catalog = new Catalog{};

  Parser parser1{"CREATE table test (x integer)"};
  Planner planner{};
  Executor e{catalog};

  std::unique_ptr<PlanNode> plan1 = planner.CreatePlan(parser1.ParseStatement().unwrap());
  ExecutionOutput res1 = e.Execute(plan1.get());

  EXPECT_EQ(res1.type, OutputType::CreateTable);
  EXPECT_EQ(res1.error, false);
  EXPECT_EQ(catalog->tables.size(), 1);
  EXPECT_EQ(catalog->ReadTable("test").unwrap()->GetColumn("x").isOk(), true);

  Parser parser2{"DROP table test"};
  std::unique_ptr<PlanNode> plan2 = planner.CreatePlan(parser2.ParseStatement().unwrap());
  ExecutionOutput res2 = e.Execute(plan2.get());

  EXPECT_EQ(res2.type, OutputType::DropTable);
  EXPECT_EQ(res2.error, false);
  EXPECT_EQ(catalog->tables.size(), 0);
}

TEST(ExecutorTest, ExecutorCreateTableTest2) {
  Catalog * catalog = new Catalog{};
  Planner planner{};
  Executor e{catalog};

  Parser parser1{"CREATE table producer (id integer, name varchar)"};
  std::unique_ptr<PlanNode> plan1 = planner.CreatePlan(parser1.ParseStatement().unwrap());
  ExecutionOutput res1 = e.Execute(plan1.get());

  EXPECT_EQ(res1.type, OutputType::CreateTable);
  EXPECT_EQ(res1.error, false);
  EXPECT_EQ(catalog->tables.size(), 1);

  Parser parser2{"CREATE table movie (id integer, producer integer foreign key references producer(badid))"};
  std::unique_ptr<PlanNode> plan2 = planner.CreatePlan(parser2.ParseStatement().unwrap());
  ExecutionOutput res2 = e.Execute(plan2.get());

  EXPECT_EQ(res2.type, OutputType::CreateTable);
  EXPECT_EQ(res2.error, true);

  Parser parser3{"CREATE table movie (id integer, producer integer foreign key references producer(id))"};
  std::unique_ptr<PlanNode> plan3 = planner.CreatePlan(parser3.ParseStatement().unwrap());
  ExecutionOutput res3 = e.Execute(plan3.get());

  EXPECT_EQ(res3.type, OutputType::CreateTable);
  EXPECT_EQ(res3.error, false);
  EXPECT_EQ(catalog->tables.size(), 2);
}
