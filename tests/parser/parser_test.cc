#include <gtest/gtest.h>

#include "parser/parser.h"
#include "parser/statement/statement.h"
#include "parser/statement/create_table.h"
#include "parser/statement/drop_table.h"
#include "parser/statement/select.h"
#include "parser/statement/insert.h"
#include "parser/statement/delete.h"
#include "parser/statement/update.h"

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
  InfixOperation * i = dynamic_cast<InfixOperation*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(i->Display(), "(4 AND 4)");
}

TEST(ParserTest, ParserExpression3) {
  Parser p{"stest OR sedt"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  InfixOperation * i = dynamic_cast<InfixOperation*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(i->Display(), "(stest OR sedt)");
}

TEST(ParserTest, ParserExpression4) {
  Parser p{"stest OR sedt AND ttt"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  InfixOperation * i = dynamic_cast<InfixOperation*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(i->Display(), "(stest OR (sedt AND ttt))");
}

TEST(ParserTest, ParserExpression5) {
  Parser p{"stest AND sedt OR ttt"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  InfixOperation * i = dynamic_cast<InfixOperation*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(i->Display(), "((stest AND sedt) OR ttt)");
}

TEST(ParserTest, ParserExpression6) {
  Parser p{"stest OR sedt OR ttt"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  InfixOperation * i = dynamic_cast<InfixOperation*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(i->Display(), "((stest OR sedt) OR ttt)");
}

TEST(ParserTest, ParserExpression7) {
  Parser p{"4 * 4 + 7"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  InfixOperation * i = dynamic_cast<InfixOperation*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(s.unwrap()->Display(), "((4 * 4) + 7)");
}

TEST(ParserTest, ParserExpression8) {
  Parser p{"4 + 4 * 7"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  InfixOperation * i = dynamic_cast<InfixOperation*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(s.unwrap()->Display(), "(4 + (4 * 7))");
}

TEST(ParserTest, ParserExpression9) {
  Parser p{"2 ^ 3 ^ 4"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  InfixOperation * i = dynamic_cast<InfixOperation*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(s.unwrap()->Display(), "(2 ^ (3 ^ 4))");
}

TEST(ParserTest, ParserExpression10) {
  Parser p{"7 * 3 ^ 4"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  InfixOperation * i = dynamic_cast<InfixOperation*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(s.unwrap()->Display(), "(7 * (3 ^ 4))");
}

TEST(ParserTest, ParserExpression11) {
  Parser p{"2 * (3 + 5) * 7"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  InfixOperation * i = dynamic_cast<InfixOperation*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(s.unwrap()->Display(), "((2 * (3 + 5)) * 7)");
}

TEST(ParserTest, ParserExpression12) {
  Parser p{"test.fielder AND 'test' = FALSE"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  InfixOperation * i = dynamic_cast<InfixOperation*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(s.unwrap()->Display(), "(test.fielder AND ('test' = FALSE))");
}

// Postfix
TEST(ParserTest, ParserExpression13) {
  Parser p{"test.fielder IS NOT NULL"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->Display(), "(test.fielder IS NOT NULL)");
}

TEST(ParserTest, ParserExpression14) {
  Parser p{"test.fielder IS NULL"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->Display(), "(test.fielder IS NULL)");
}

TEST(ParserTest, ParserExpression15) {
  Parser p{"test.fielder!! = NULL"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->Display(), "(((test.fielder)!)! = NULL)");
}

TEST(ParserTest, ParserExpression16) {
  Parser p{"(test.fielder) IS NULL AND 4!"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->Display(), "((test.fielder IS NULL) AND (4)!)");
}

TEST(ParserTest, ParserExpression17) {
  Parser p{"(test.fielder) IS FALSE NULL AND 4!"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), false);
}

TEST(ParserTest, ParserExpression18) {
  Parser p{"(test.fielder) IS NOT FALSE"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), false);
}

// Prefix
TEST(ParserTest, ParserExpression19) {
  Parser p{"+4 + -4"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->Display(), "(ASSERT(4) + NEGATE(4))");
}

TEST(ParserTest, ParserExpression20) {
  Parser p{"NOT 4! + --4"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->Display(), "((NOT(4))! + NEGATE(NEGATE(4)))");
}

TEST(ParserTest, ParserSelect1) {
  Parser p{"SELECT * FROM test"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
}

TEST(ParserTest, ParserSelect2) {
  Parser p{"SELECT id FROM hey where 4 =4"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
}

TEST(ParserTest, ParserSelect3) {
  Parser p{"SELECT id, what.id FROM hey, what where 4 =4"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
}

TEST(ParserTest, ParserSelect4) {
  Parser p{"SELECT id, what.id FROM hey, what group by what.id"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
}

TEST(ParserTest, ParserSelect5) {
  Parser p{"SELECT id, what.id FROM hey, what order by what.id, name"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
}

TEST(ParserTest, ParserSelect6) {
  Parser p{"SELECT id, what.id FROM hey, what group by what.id, name"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
}

TEST(ParserTest, ParserSelect7) {
  Parser p{"SELECT id, what.id FROM hey, what order by what.id, name ASC, birth DESC"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
}

TEST(ParserTest, ParserSelect8) {
  Parser p{"SELECT id, what.id FROM hey, what group by what.id having birth='30-30-30'"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
}

TEST(ParserTest, ParserSelect9) {
  Parser p{"SELECT id, what.id FROM hey, what limit 98"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
}

TEST(ParserTest, ParserSelect10) {
  Parser p{"SELECT id, what.id FROM hey, what offset 98"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
}

TEST(ParserTest, ParserInsert1) {
  Parser p{"INSERT INTO test VALUES (test, yes)"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
  InsertStatement * i = dynamic_cast<InsertStatement*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(i->table, "test");
  EXPECT_EQ(i->values.size(), 1);
  EXPECT_EQ(i->values.at(0).at(0)->Display(), "test");
  EXPECT_EQ(i->values.at(0).at(1)->Display(), "yes");
}

TEST(ParserTest, ParserInsert2) {
  Parser p{"INSERT INTO test VALUES (test, yes), (34, 78)"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
  InsertStatement * i = dynamic_cast<InsertStatement*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(i->table, "test");
  EXPECT_EQ(i->values.size(), 2);
  EXPECT_EQ(i->values.at(0).at(0)->Display(), "test");
  EXPECT_EQ(i->values.at(0).at(1)->Display(), "yes");
  EXPECT_EQ(i->values.at(1).at(0)->Display(), "34");
  EXPECT_EQ(i->values.at(1).at(1)->Display(), "78");
}

TEST(ParserTest, ParserInsert3) {
  Parser p{"INSERT INTO test (col1, col2) VALUES (test, yes)"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
  InsertStatement * i = dynamic_cast<InsertStatement*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(i->table, "test");
  EXPECT_EQ(i->columns.at(0), "col1");
  EXPECT_EQ(i->columns.at(1), "col2");
  EXPECT_EQ(i->values.size(), 1);
  EXPECT_EQ(i->values.at(0).at(0)->Display(), "test");
  EXPECT_EQ(i->values.at(0).at(1)->Display(), "yes");
}

TEST(ParserTest, ParserInsert4) {
  Parser p{"INSERT INTO  test  (col1) VALUES (test, yes), (34, 78)"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
  InsertStatement * i = dynamic_cast<InsertStatement*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(i->table, "test");
  EXPECT_EQ(i->columns.at(0), "col1");
  EXPECT_EQ(i->values.size(), 2);
  EXPECT_EQ(i->values.at(0).at(0)->Display(), "test");
  EXPECT_EQ(i->values.at(0).at(1)->Display(), "yes");
  EXPECT_EQ(i->values.at(1).at(0)->Display(), "34");
  EXPECT_EQ(i->values.at(1).at(1)->Display(), "78");
}

TEST(ParserTest, ParserDelete1) {
  Parser p{"DELETE FROM test where true"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
  DeleteStatement * i = dynamic_cast<DeleteStatement*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(i->table, "test");
  EXPECT_EQ(i->where->Display(), "TRUE");
}

TEST(ParserTest, ParserDelete2) {
  Parser p{"DELETE FROM test where true and false"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
  DeleteStatement * i = dynamic_cast<DeleteStatement*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(i->table, "test");
  EXPECT_EQ(i->where->Display(), "(TRUE AND FALSE)");
}

TEST(ParserTest, ParserDelete3) {
  Parser p{"DELETE FROM test"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
  DeleteStatement * i = dynamic_cast<DeleteStatement*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(i->table, "test");
}

TEST(ParserTest, ParserUpdate1) {
  Parser p{"UPDATE test SET r=x, g=5 where true"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
  UpdateStatement * i = dynamic_cast<UpdateStatement*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(i->table, "test");
  EXPECT_EQ(i->set["r"]->Display(), "x");
  EXPECT_EQ(i->set["g"]->Display(), "5");
  EXPECT_EQ(i->where->Display(), "TRUE");
}

TEST(ParserTest, ParserUpdate2) {
  Parser p{"UPDATE test SET r=8"};
  auto s = p.ParseStatement();
  EXPECT_EQ(s.isOk(), true);
  UpdateStatement * i = dynamic_cast<UpdateStatement*>(s.unwrap());
  EXPECT_NE(i, nullptr);
  EXPECT_EQ(i->table, "test");
  EXPECT_EQ(i->set["r"]->Display(), "8");
}

