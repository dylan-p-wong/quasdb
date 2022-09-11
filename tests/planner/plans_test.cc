#include <gtest/gtest.h>

#include "storage/catalog/catalog.h"
#include "planner/planner.h"
#include "parser/parser.h"
#include "planner/plans/create_table_plan.h"
#include "planner/plans/drop_table_plan.h"

TEST(PlansTest, PlansCreateTableTest1) {
  Catalog * catalog = new Catalog{};
  Parser parser{"CREATE table test (x integer)"};
  Planner planner{};
  std::unique_ptr<PlanNode> plan = planner.CreatePlan(parser.ParseStatement().unwrap());
  
  const CreateTablePlan * create_table_plan = dynamic_cast<const CreateTablePlan*>(plan.get());
  EXPECT_NE(create_table_plan, nullptr);
  const DropTablePlan * drop_table_plan = dynamic_cast<const DropTablePlan*>(plan.get());
  EXPECT_EQ(drop_table_plan, nullptr);

  EXPECT_EQ(create_table_plan->table->name, "test");

  std::vector<CatalogColumn*> columns = create_table_plan->table->columns;

  EXPECT_EQ(columns.size(), 1);
  EXPECT_EQ(columns.at(0)->name, "x");
  EXPECT_EQ(columns.at(0)->primary_key, false);
  EXPECT_EQ(columns.at(0)->nullable, true);
  EXPECT_EQ(columns.at(0)->default_value, nullptr);
  EXPECT_EQ(columns.at(0)->unique, false);
  EXPECT_EQ(columns.at(0)->index, false);
  // Still need references (need binder)
}
