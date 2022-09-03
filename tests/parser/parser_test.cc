#include <gtest/gtest.h>

#include "parser/parser.h"
#include "parser/statement/statement.h"
#include "parser/statement/create_table.h"
#include "parser/statement/drop_table.h"

TEST(ParserTest, ParserInvalidStatement) {
  Parser p{"DERP SELECT * FROM movies"};
  EXPECT_EQ(p.ParseStatement().isErr(), true);
  EXPECT_EQ(p.ParseStatement().unwrapErr().message, "Unexpected token DERP");
}

TEST(ParserTest, ParserinvalidCreateTable) {
  Parser p{"CREATE test ()"};
  EXPECT_EQ(p.ParseStatement().isOk(), false);
  p = Parser{"CREATE TABLE 'what' ()"};
  EXPECT_EQ(p.ParseStatement().isOk(), false);
  p = Parser{"CREATE TABLE ()"};
  EXPECT_EQ(p.ParseStatement().isOk(), false);
  p = Parser{"CREATE TABLE test ("};
  EXPECT_EQ(p.ParseStatement().isOk(), false);
}

TEST(ParserTest, ParserValidCreateTable) {
  Parser p{"CREATE TABLE test ()"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), false);
}

TEST(ParserTest, ParserBasicCreateTable) {
  Parser p{"CREATE TABLE test (x integer)"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->type, StatementType::CreateTable);
  CreateTable * c = dynamic_cast<CreateTable*>(s.unwrap());
  EXPECT_EQ(c->name, "test");
  EXPECT_EQ(c->columns.size(), 1);
  EXPECT_EQ(c->columns.at(0).data_type, DataType::Integer);
}

TEST(ParserTest, ParserBasicCreateTable2) {
  Parser p{"CREATE TABLE testing (x integer, y varchar)"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->type, StatementType::CreateTable);
  CreateTable * c = dynamic_cast<CreateTable*>(s.unwrap());
  EXPECT_EQ(c->name, "testing");
  EXPECT_EQ(c->columns.size(), 2);
  EXPECT_EQ(c->columns.at(0).data_type, DataType::Integer);
  EXPECT_EQ(c->columns.at(1).data_type, DataType::Varchar);
}

TEST(ParserTest, ParserBasicCreateTable3) {
  Parser p{"CREATE TABLE testing (x integer PRIMARY KEY UNIQUE, y varchar)"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->type, StatementType::CreateTable);
  CreateTable * c = dynamic_cast<CreateTable*>(s.unwrap());
  EXPECT_EQ(c->name, "testing");
  EXPECT_EQ(c->columns.size(), 2);
  EXPECT_EQ(c->columns.at(0).primary_key, true);
  EXPECT_EQ(c->columns.at(0).unique, true);
}

TEST(ParserTest, ParserInvalidDropTable) {
  Parser p{"DROP TABLE 'test'"};
  EXPECT_EQ(p.ParseStatement().isErr(), true);
  p = Parser{"DROP TABLE ."};
  EXPECT_EQ(p.ParseStatement().isErr(), true);
}

TEST(ParserTest, ParserValidDropTable) {
  Parser p{"DROP TABLE test"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->type, StatementType::DropTable);
  DropTable * d = dynamic_cast<DropTable*>(s.unwrap());
  EXPECT_EQ(d->name, "test");
}
