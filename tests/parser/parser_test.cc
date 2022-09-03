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
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->type, StatementType::CreateTable);
  CreateTable * c = dynamic_cast<CreateTable*>(s.unwrap());
  EXPECT_EQ(c->name, "test");
  EXPECT_EQ(c->columns.size(), 0);
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
