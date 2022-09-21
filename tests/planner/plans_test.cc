#include <gtest/gtest.h>

#include "storage/catalog/catalog.h"
#include "planner/planner.h"
#include "parser/parser.h"

#include "planner/plans/create_table_plan.h"
#include "planner/plans/drop_table_plan.h"
#include "planner/plans/insert_plan.h"

TEST(PlansTest, PlansCreateTableTest1) {
  Catalog * catalog = new Catalog{nullptr};
  Parser parser{"CREATE table test (x integer)"};
  Planner planner{catalog};
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
  EXPECT_EQ(create_table_plan->table->GetReferences().size(), 0);
}

TEST(PlansTest, PlansInsertTest1) {
  Catalog * catalog = new Catalog{nullptr};
  Planner planner{catalog};

  Parser parser1{"CREATE table test (x integer)"};
  std::unique_ptr<PlanNode> plan1 = planner.CreatePlan(parser1.ParseStatement().unwrap());
  
  Parser parser2{"INSERT INTO test VALUES (1 * 8, 7^2)"};
  std::unique_ptr<PlanNode> plan2 = planner.CreatePlan(parser2.ParseStatement().unwrap());

  const InsertPlan * insert_plan = dynamic_cast<const InsertPlan*>(plan2.get());
  EXPECT_NE(insert_plan, nullptr);
  EXPECT_EQ(insert_plan->table, "test");
  EXPECT_EQ(insert_plan->values.at(0).at(0)->type, DataType::Integer);
  EXPECT_EQ(insert_plan->values.at(0).at(1)->type, DataType::Integer);

  EXPECT_EQ(dynamic_cast<const Data<int>*>(insert_plan->values.at(0).at(0).get())->value, 8);
  EXPECT_EQ(dynamic_cast<const Data<int>*>(insert_plan->values.at(0).at(1).get())->value, 49);
}
