#include <gtest/gtest.h>

#include "parser/lexer.h"

TEST(LexerTest, LexerBasicAssertions) {
  Lexer l{"SELECT * FROM movies"};
}
