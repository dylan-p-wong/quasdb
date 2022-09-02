#include <gtest/gtest.h>
#include <iostream>

#include "parser/lexer.h"

TEST(LexerTest, LexerSelect) {
  Lexer l{"SELECT * FROM movies"};
  std::optional<Result<Token>> t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::Select);
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::Asterisk);
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::From);
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::IdentifierValue);
  EXPECT_EQ(t.value().value.value, "movies");
  t = l.Scan();
  EXPECT_EQ(t.has_value(), false);
}

TEST(LexerTest, LexerSelect2) {
  Lexer l{"SELECT id,title FROM movies where id=40"};
  std::optional<Result<Token>> t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::Select);
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::IdentifierValue);
  EXPECT_EQ(t.value().value.value, "id");
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::Comma);
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::IdentifierValue);
  EXPECT_EQ(t.value().value.value, "title");
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::From);
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::IdentifierValue);
  EXPECT_EQ(t.value().value.value, "movies");
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::Where);
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::IdentifierValue);
  EXPECT_EQ(t.value().value.value, "id");
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::Equal);
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::NumberValue);
  EXPECT_EQ(t.value().value.value, "40");
  t = l.Scan();
  EXPECT_EQ(t.has_value(), false);
}

TEST(LexerTest, LexerIdentifierQuoted) {
  Lexer l{"\"movies\""};
  std::optional<Result<Token>> t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::IdentifierValue);
  EXPECT_EQ(t.value().value.value, "movies");
  t = l.Scan();
  EXPECT_EQ(t.has_value(), false);
}

TEST(LexerTest, LexerIdentifierQuoted2) {
  Lexer l{"SELECT FROM \"mov#@$DFA SDfies\""};
  std::optional<Result<Token>> t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::Select);
  EXPECT_EQ(t.value().value.value, "SELECT");
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::From);
  EXPECT_EQ(t.value().value.value, "FROM");
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::IdentifierValue);
  EXPECT_EQ(t.value().value.value, "mov#@$DFA SDfies");
  t = l.Scan();
  EXPECT_EQ(t.has_value(), false);
}

TEST(LexerTest, LexerString) {
  Lexer l{"'movies'"};
  std::optional<Result<Token>> t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::StringValue);
  EXPECT_EQ(t.value().value.value, "movies");
  t = l.Scan();
  EXPECT_EQ(t.has_value(), false);
}

TEST(LexerTest, LexerString2) {
  Lexer l{"SELECT FROM 'movf asdf #$@# 432432 ies'"};
  std::optional<Result<Token>> t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::Select);
  EXPECT_EQ(t.value().value.value, "SELECT");
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::From);
  EXPECT_EQ(t.value().value.value, "FROM");
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::StringValue);
  EXPECT_EQ(t.value().value.value, "movf asdf #$@# 432432 ies");
  t = l.Scan();
  EXPECT_EQ(t.has_value(), false);
}

TEST(LexerTest, LexerScanSymbol) {
  Lexer l{".    ="};
  std::optional<Result<Token>> t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::Period);
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::Equal);
}

TEST(LexerTest, LexerScanNumber) {
  Lexer l{"9000234234fasdf"};
  std::optional<Result<Token>> t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::NumberValue);
  EXPECT_EQ(t.value().value.value, "9000234234");
  t = l.Scan();
  EXPECT_EQ(t.value().value.type, TokenType::IdentifierValue);
  EXPECT_EQ(t.value().value.value, "fasdf");
}
