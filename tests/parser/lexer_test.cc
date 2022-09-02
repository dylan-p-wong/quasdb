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
