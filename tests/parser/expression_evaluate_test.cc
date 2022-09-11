#include <gtest/gtest.h>

#include "parser/parser.h"

TEST(ExpressionTest, ExpressionEvaluateBasic1) {
  Parser p{"4"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->EvaluateConstant()->type, DataType::Integer);
  EXPECT_EQ(dynamic_cast<const Data<int>*>(s.unwrap()->EvaluateConstant().get())->value, 4);
}

TEST(ExpressionTest, ExpressionEvaluateBasic2) {
  Parser p{"'test'"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->EvaluateConstant()->type, DataType::Varchar);
  EXPECT_EQ(dynamic_cast<const Data<std::string>*>(s.unwrap()->EvaluateConstant().get())->value, "test");
}

TEST(ExpressionTest, ExpressionEvaluateBasic3) {
  Parser p{"INFINITY"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->EvaluateConstant()->type, DataType::Float);
}

TEST(ExpressionTest, ExpressionEvaluateBasic4) {
  Parser p{"true"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->EvaluateConstant()->type, DataType::Boolean);
  EXPECT_EQ(dynamic_cast<const Data<bool>*>(s.unwrap()->EvaluateConstant().get())->value, true);
}

TEST(ExpressionTest, ExpressionEvaluateBasic5) {
  Parser p{"NULL"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->EvaluateConstant().get()->type, DataType::Null);
}

TEST(ExpressionTest, ExpressionEvaluateSubtraction1) {
  Parser p{"4 - 3"};
  auto s = p.ParseExpression(0);
  EXPECT_EQ(s.isOk(), true);
  EXPECT_EQ(s.unwrap()->EvaluateConstant().get()->type, DataType::Integer);
  EXPECT_EQ(dynamic_cast<const Data<int>*>(s.unwrap()->EvaluateConstant().get())->value, 1);
}
