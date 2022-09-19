#include <gtest/gtest.h>

#include "storage/catalog/catalog.h"
#include "planner/planner.h"
#include "parser/parser.h"
#include "planner/plans/create_table_plan.h"
#include "planner/plans/drop_table_plan.h"
#include "storage/buffer/buffer_manager.h"
#include "executor/executor.h"

TEST(ExecutorTest, ExecutorCreateTableTest1) {
  BufferManager * buffer_manager = new BufferManager{};
  Catalog * catalog = new Catalog{buffer_manager};

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
  BufferManager * buffer_manager = new BufferManager{};
  Catalog * catalog = new Catalog{buffer_manager};
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

TEST(ExecutorTest, ExecutorInsertTest1) {
  BufferManager * buffer_manager = new BufferManager{};
  Catalog * catalog = new Catalog{buffer_manager};
  Planner planner{};
  Executor e{catalog};

  Parser parser1{"CREATE table test (x integer, y integer)"};
  std::unique_ptr<PlanNode> plan1 = planner.CreatePlan(parser1.ParseStatement().unwrap());
  ExecutionOutput res1 = e.Execute(plan1.get());

  EXPECT_EQ(res1.type, OutputType::CreateTable);
  EXPECT_EQ(res1.error, false);
  EXPECT_EQ(catalog->tables.size(), 1);
  EXPECT_EQ(catalog->tables.at(0)->GetNumberOfColumns(), 2);

  Parser parser2{"INSERT INTO test VALUES (1 * 8, 7^2)"};
  std::unique_ptr<PlanNode> plan2 = planner.CreatePlan(parser2.ParseStatement().unwrap());
  ExecutionOutput res2 = e.Execute(plan2.get());

  EXPECT_EQ(buffer_manager->next_page_id, 2);
  DirectoryPage * dp = reinterpret_cast<DirectoryPage*>(buffer_manager->GetPage(0));
  EXPECT_EQ(dp->GetDataPagePageId(0), 1);
  EXPECT_EQ(dp->GetDataPageFreeSpace(0), 4068);

  TablePage * tp = reinterpret_cast<TablePage*>(buffer_manager->GetPage(1));
  EXPECT_EQ(tp->GetTupleCount(), 1);
  EXPECT_EQ(tp->GetFreeSpace(), 4068);
  EXPECT_EQ(tp->GetTupleInfo(0).first, 4088);
  EXPECT_EQ(tp->GetTupleInfo(0).second, 8);
  EXPECT_EQ(*reinterpret_cast<int*>(tp->GetData() + 4088), 8);
  EXPECT_EQ(*reinterpret_cast<int*>(tp->GetData() + 4088 + 4), 49);

  auto tuple1 = tp->GetTuple(RID{1, 0}, catalog->tables.at(0)).unwrap();
  auto data1 = tuple1->GetValueAtColumnIndex(0, catalog->tables.at(0));
  EXPECT_EQ(data1->type, DataType::Integer);
  Data<int> * int_data1 = dynamic_cast<Data<int>*>(data1.get());
  EXPECT_EQ(int_data1->value, 8);

  auto tuple2 = tp->GetTuple(RID{1, 0}, catalog->tables.at(0)).unwrap();
  auto data2 = tuple2->GetValueAtColumnIndex(1, catalog->tables.at(0));
  EXPECT_EQ(data2->type, DataType::Integer);
  Data<int> * int_data2 = dynamic_cast<Data<int>*>(data2.get());
  EXPECT_EQ(int_data2->value, 49);
}

TEST(ExecutorTest, ExecutorInsertTest2) {
  BufferManager * buffer_manager = new BufferManager{};
  Catalog * catalog = new Catalog{buffer_manager};
  Planner planner{};
  Executor e{catalog};

  Parser parser1{"CREATE table test (x integer, y integer)"};
  std::unique_ptr<PlanNode> plan1 = planner.CreatePlan(parser1.ParseStatement().unwrap());
  ExecutionOutput res1 = e.Execute(plan1.get());

  EXPECT_EQ(res1.type, OutputType::CreateTable);
  EXPECT_EQ(res1.error, false);
  EXPECT_EQ(catalog->tables.size(), 1);
  EXPECT_EQ(catalog->tables.at(0)->GetNumberOfColumns(), 2);

  Parser parser2{"INSERT INTO test VALUES ('test', 7^2)"};
  std::unique_ptr<PlanNode> plan2 = planner.CreatePlan(parser2.ParseStatement().unwrap());
  ExecutionOutput res2 = e.Execute(plan2.get());
  EXPECT_EQ(res2.error, true);

  Parser parser3{"INSERT INTO test VALUES (6, NULL)"};
  std::unique_ptr<PlanNode> plan3 = planner.CreatePlan(parser3.ParseStatement().unwrap());
  ExecutionOutput res3 = e.Execute(plan3.get());
  EXPECT_EQ(res3.error, true);

  Parser parser4{"INSERT INTO test VALUES (6, 7)"};
  std::unique_ptr<PlanNode> plan4 = planner.CreatePlan(parser4.ParseStatement().unwrap());
  ExecutionOutput res4 = e.Execute(plan4.get());
  EXPECT_EQ(res4.error, false);
}

TEST(ExecutorTest, ExecutorSelectTest1) {
  BufferManager * buffer_manager = new BufferManager{};
  Catalog * catalog = new Catalog{buffer_manager};
  Planner planner{};
  Executor e{catalog};

  Parser parser1{"CREATE table test (x integer, y integer)"};
  std::unique_ptr<PlanNode> plan1 = planner.CreatePlan(parser1.ParseStatement().unwrap());
  ExecutionOutput res1 = e.Execute(plan1.get());

  EXPECT_EQ(res1.type, OutputType::CreateTable);
  EXPECT_EQ(res1.error, false);
  EXPECT_EQ(catalog->tables.size(), 1);
  EXPECT_EQ(catalog->tables.at(0)->GetNumberOfColumns(), 2);

  Parser parser2{"INSERT INTO test VALUES (1, 2)"};
  std::unique_ptr<PlanNode> plan2 = planner.CreatePlan(parser2.ParseStatement().unwrap());
  ExecutionOutput res2 = e.Execute(plan2.get());
  EXPECT_EQ(res2.error, false);

  Parser parser3{"INSERT INTO test VALUES (3, 4)"};
  std::unique_ptr<PlanNode> plan3 = planner.CreatePlan(parser3.ParseStatement().unwrap());
  ExecutionOutput res3 = e.Execute(plan3.get());
  EXPECT_EQ(res3.error, false);

  Parser parser4{"INSERT INTO test VALUES (6, 7)"};
  std::unique_ptr<PlanNode> plan4 = planner.CreatePlan(parser4.ParseStatement().unwrap());
  ExecutionOutput res4 = e.Execute(plan4.get());
  EXPECT_EQ(res4.error, false);

  // Parser parser5{"SELECT * FROM test"};
  // std::unique_ptr<PlanNode> plan5 = planner.CreatePlan(parser5.ParseStatement().unwrap());
  // ExecutionOutput res5 = e.Execute(plan5.get());
  // EXPECT_EQ(res5.error, false);

  auto tuple1 = catalog->tables.at(0)->GetTuple(RID{1, 0}, buffer_manager).unwrap();
  auto data1 = tuple1->GetValueAtColumnIndex(0, catalog->tables.at(0));
  EXPECT_EQ(data1->type, DataType::Integer);
  Data<int> * int_data1 = dynamic_cast<Data<int>*>(data1.get());
  EXPECT_EQ(int_data1->value, 1);

  
  EXPECT_EQ((dynamic_cast<Data<int>*>((catalog->tables.at(0)->GetTuple(RID{1, 0}, buffer_manager).unwrap()->GetValueAtColumnIndex(1, catalog->tables.at(0))).get()))->value, 2);
  EXPECT_EQ((dynamic_cast<Data<int>*>((catalog->tables.at(0)->GetTuple(RID{1, 1}, buffer_manager).unwrap()->GetValueAtColumnIndex(0, catalog->tables.at(0))).get()))->value, 3);
  EXPECT_EQ((dynamic_cast<Data<int>*>((catalog->tables.at(0)->GetTuple(RID{1, 1}, buffer_manager).unwrap()->GetValueAtColumnIndex(1, catalog->tables.at(0))).get()))->value, 4);
  EXPECT_EQ((dynamic_cast<Data<int>*>((catalog->tables.at(0)->GetTuple(RID{1, 2}, buffer_manager).unwrap()->GetValueAtColumnIndex(0, catalog->tables.at(0))).get()))->value, 6);
  EXPECT_EQ((dynamic_cast<Data<int>*>((catalog->tables.at(0)->GetTuple(RID{1, 2}, buffer_manager).unwrap()->GetValueAtColumnIndex(1, catalog->tables.at(0))).get()))->value, 7);
  EXPECT_EQ(catalog->tables.at(0)->GetTuple(RID{1, 3}, buffer_manager).isErr(), true);
  EXPECT_EQ(catalog->tables.at(0)->GetTuple(RID{2, 0}, buffer_manager).isErr(), true);
  EXPECT_EQ(catalog->tables.at(0)->GetTuple(RID{0, 0}, buffer_manager).isErr(), true);
}

TEST(ExecutorTest, ExecutorSelectTest2) {
  BufferManager * buffer_manager = new BufferManager{};
  Catalog * catalog = new Catalog{buffer_manager};
  Planner planner{};
  Executor e{catalog};

  Parser parser1{"CREATE table test (x integer, y integer, z integer)"};
  std::unique_ptr<PlanNode> plan1 = planner.CreatePlan(parser1.ParseStatement().unwrap());
  ExecutionOutput res1 = e.Execute(plan1.get());

  EXPECT_EQ(res1.type, OutputType::CreateTable);
  EXPECT_EQ(res1.error, false);
  EXPECT_EQ(catalog->tables.size(), 1);
  EXPECT_EQ(catalog->tables.at(0)->GetNumberOfColumns(), 3);

  Parser parser2{"INSERT INTO test VALUES (1, 2, 3)"};
  std::unique_ptr<PlanNode> plan2 = planner.CreatePlan(parser2.ParseStatement().unwrap());
  ExecutionOutput res2 = e.Execute(plan2.get());
  EXPECT_EQ(res2.error, false);

  Parser parser3{"INSERT INTO test VALUES (4, 5, 6)"};
  std::unique_ptr<PlanNode> plan3 = planner.CreatePlan(parser3.ParseStatement().unwrap());
  ExecutionOutput res3 = e.Execute(plan3.get());
  EXPECT_EQ(res3.error, false);

  Parser parser4{"INSERT INTO test VALUES (7, 8, 9)"};
  std::unique_ptr<PlanNode> plan4 = planner.CreatePlan(parser4.ParseStatement().unwrap());
  ExecutionOutput res4 = e.Execute(plan4.get());
  EXPECT_EQ(res4.error, false);

  Parser parser5{"SELECT * FROM test"};
  std::unique_ptr<PlanNode> plan5 = planner.CreatePlan(parser5.ParseStatement().unwrap());
  ExecutionOutput res5 = e.Execute(plan5.get());
  EXPECT_EQ(res5.error, false);
  EXPECT_EQ(res5.rows.size(), 3);
  EXPECT_EQ(res5.rows.at(0).size(), 3);
  EXPECT_EQ(res5.rows.at(1).size(), 3);
  EXPECT_EQ(res5.rows.at(2).size(), 3);
  EXPECT_EQ(dynamic_cast<Data<int>*>(res5.rows.at(0).at(0))->value, 1);
  EXPECT_EQ(dynamic_cast<Data<int>*>(res5.rows.at(0).at(1))->value, 2);
  EXPECT_EQ(dynamic_cast<Data<int>*>(res5.rows.at(0).at(2))->value, 3);
  EXPECT_EQ(dynamic_cast<Data<int>*>(res5.rows.at(1).at(0))->value, 4);
  EXPECT_EQ(dynamic_cast<Data<int>*>(res5.rows.at(1).at(1))->value, 5);
  EXPECT_EQ(dynamic_cast<Data<int>*>(res5.rows.at(1).at(2))->value, 6);
  EXPECT_EQ(dynamic_cast<Data<int>*>(res5.rows.at(2).at(0))->value, 7);
  EXPECT_EQ(dynamic_cast<Data<int>*>(res5.rows.at(2).at(1))->value, 8);
  EXPECT_EQ(dynamic_cast<Data<int>*>(res5.rows.at(2).at(2))->value, 9);

  ExecutionOutput res6 = e.Execute(plan4.get());
  ExecutionOutput res7 = e.Execute(plan5.get());
  EXPECT_EQ(dynamic_cast<Data<int>*>(res7.rows.at(3).at(0))->value, 7);
  EXPECT_EQ(dynamic_cast<Data<int>*>(res7.rows.at(3).at(1))->value, 8);
  EXPECT_EQ(dynamic_cast<Data<int>*>(res7.rows.at(3).at(2))->value, 9);
}
