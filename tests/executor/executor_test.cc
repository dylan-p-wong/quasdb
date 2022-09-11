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

  Parser parser2{"DROP table test"};
  std::unique_ptr<PlanNode> plan2 = planner.CreatePlan(parser2.ParseStatement().unwrap());
  ExecutionOutput res2 = e.Execute(plan2.get());

  EXPECT_EQ(res2.type, OutputType::DropTable);
  EXPECT_EQ(res2.error, false);
  EXPECT_EQ(catalog->tables.size(), 0);
}
