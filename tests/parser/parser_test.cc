#include <gtest/gtest.h>

#include "parser/parser.h"

TEST(ParserTest, ParserInvalidStatement) {
  Parser p{"DERP SELECT * FROM movies"};
  EXPECT_EQ(p.ParseStatement().isErr(), true);
  EXPECT_EQ(p.ParseStatement().unwrapErr().message, "Unexpected token DERP");
}
