#include <gtest/gtest.h>

#include "parser/lexer.h"

TEST(LexerTest, LexerSelect) {
  Lexer l{"SELECT * FROM movies"};
  Token t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::Select);
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::Asterisk);
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::From);
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::IdentifierValue);
  EXPECT_EQ(t.value, "movies");

  EXPECT_EQ(l.Scan().has_value(), false);
}

TEST(LexerTest, LexerSelect2) {
  Lexer l{"SELECT id,title FROM movies where id=40"};
  Token t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::Select);
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::IdentifierValue);
  EXPECT_EQ(t.value, "id");
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::Comma);
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::IdentifierValue);
  EXPECT_EQ(t.value, "title");
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::From);
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::IdentifierValue);
  EXPECT_EQ(t.value, "movies");
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::Where);
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::IdentifierValue);
  EXPECT_EQ(t.value, "id");
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::Equal);
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::IntegerValue);
  EXPECT_EQ(t.value, "40");

  EXPECT_EQ(l.Scan().has_value(), false);
}

TEST(LexerTest, LexerIdentifierQuoted) {
  Lexer l{"\"movies\""};
  Token t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::IdentifierValue);
  EXPECT_EQ(t.value, "movies");

  EXPECT_EQ(l.Scan().has_value(), false);
}

TEST(LexerTest, LexerIdentifierQuoted2) {
  Lexer l{"SELECT FROM \"mov#@$DFA SDfies\""};
  Token t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::Select);
  EXPECT_EQ(t.value, "SELECT");
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::From);
  EXPECT_EQ(t.value, "FROM");
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::IdentifierValue);
  EXPECT_EQ(t.value, "mov#@$DFA SDfies");

  EXPECT_EQ(l.Scan().has_value(), false);
}

TEST(LexerTest, LexerString) {
  Lexer l{"'movies'"};
  Token t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::StringValue);
  EXPECT_EQ(t.value, "movies");
  
  EXPECT_EQ(l.Scan().has_value(), false);
}

TEST(LexerTest, LexerString2) {
  Lexer l{"SELECT FROM 'movf asdf #$@# 432432 ies'"};
  Token t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::Select);
  EXPECT_EQ(t.value, "SELECT");
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::From);
  EXPECT_EQ(t.value, "FROM");
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::StringValue);
  EXPECT_EQ(t.value, "movf asdf #$@# 432432 ies");
  
  EXPECT_EQ(l.Scan().has_value(), false);
}

TEST(LexerTest, LexerScanSymbol) {
  Lexer l{".    ="};
  Token t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::Period);
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::Equal);
}

TEST(LexerTest, LexerScanNumber) {
  Lexer l{"9000234234fasdf"};
  Token t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::IntegerValue);
  EXPECT_EQ(t.value, "9000234234");
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::IdentifierValue);
  EXPECT_EQ(t.value, "fasdf");
}

TEST(LexerTest, LexerScanDecimal) {
  Lexer l{"900023.329342 324.234 -34.44"};
  Token t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::DecimalValue);
  EXPECT_EQ(t.value, "900023.329342");
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::DecimalValue);
  EXPECT_EQ(t.value, "324.234");
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::Minus);
  t = l.Scan().value().unwrap();
  EXPECT_EQ(t.type, TokenType::DecimalValue);
  EXPECT_EQ(t.value, "34.44");
}

TEST(LexerTest, LexerIterator1) {
  LexerIterator l{". ! SELECT INSERT WHAT 'testing' 1299 / string FOREIGN"};
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::Period);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::Period);
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::Exclamation);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::Exclamation);
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::Select);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::Select);
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::Insert);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::Insert);
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::IdentifierValue);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::IdentifierValue);
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::StringValue);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::StringValue);
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::IntegerValue);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::IntegerValue);
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::Slash);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::Slash);
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::String);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::String);
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::Foreign);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::Foreign);
  EXPECT_EQ(l.Peek().isErr(), true);
  EXPECT_EQ(l.Next().isErr(), true);
}

TEST(LexerTest, LexerIterator2) {
  LexerIterator l{"4 * 4 + 7"};
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::IntegerValue);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::IntegerValue);
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::Asterisk);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::Asterisk);
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::IntegerValue);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::IntegerValue);
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::Plus);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::Plus);
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::IntegerValue);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::IntegerValue);
  EXPECT_EQ(l.Peek().isErr(), true);
  EXPECT_EQ(l.Next().isErr(), true);
}

TEST(LexerTest, LexerIterator3) {
  LexerIterator l{"4.69 * 4.111 + 7.9349"};
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::DecimalValue);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::DecimalValue);
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::Asterisk);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::Asterisk);
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::DecimalValue);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::DecimalValue);
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::Plus);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::Plus);
  EXPECT_EQ(l.Peek().unwrap().type, TokenType::DecimalValue);
  EXPECT_EQ(l.Next().unwrap().type, TokenType::DecimalValue);
  EXPECT_EQ(l.Peek().isErr(), true);
  EXPECT_EQ(l.Next().isErr(), true);
}

