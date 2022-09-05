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

TEST(ParserTest, ParserBasicCreateTable4) {
  Parser p{"CREATE TABLE testing (x integer FOREIGN KEY REFERENCES derp(x), y varchar)"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->type, StatementType::CreateTable);
  CreateTable * c = dynamic_cast<CreateTable*>(s.unwrap());
  EXPECT_EQ(c->columns.at(0).references.value().first, "derp");
  EXPECT_EQ(c->columns.at(0).references.value().second, "x");
}

TEST(ParserTest, ParserBasicCreateTableDefault) {
  Parser p{"CREATE TABLE testing (x integer DEFAULT 4)"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->type, StatementType::CreateTable);
  CreateTable * c = dynamic_cast<CreateTable*>(s.unwrap());
  IntegerLiteral * i = dynamic_cast<IntegerLiteral*>(c->columns.at(0).default_value);
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(i->value, 4);
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

TEST(ParserTest, ParserExpression1) {
  Parser p{"4"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  IntegerLiteral * i = dynamic_cast<IntegerLiteral*>(s.unwrap());
  EXPECT_NE(i, nullptr);
}

TEST(ParserTest, ParserExpression2) {
  Parser p{"4 AND 4"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  Infix * i = dynamic_cast<Infix*>(s.unwrap());
  EXPECT_NE(i, nullptr);
}

TEST(ParserTest, ParserExpression3) {
  Parser p{"stest OR sedt"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  Infix * i = dynamic_cast<Infix*>(s.unwrap());
  EXPECT_NE(i, nullptr);
}
