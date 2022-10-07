#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace interpreter {
class Context {
private:
  std::string input;
  std::string output;

public:
  void SetInput(std::string i) { input = std::move(i); }

  std::string GetInput() { return input; }

  void SetOutput(std::string o) { output = std::move(o); }

  std::string GetOutput() { return output; }
};

class AbstractExpression {
public:
  virtual void Interpreter(Context *context) = 0;
  virtual ~AbstractExpression() = default;
};

class TerminalExpression : public AbstractExpression {
public:
  void Interpreter(Context *context) override {
    std::cout << "TerminalExpression: " << context->GetInput() << ", "
              << context->GetOutput() << std::endl;
  }
};

class NonterminalExpression : public AbstractExpression {
private:
  AbstractExpression *expression;

public:
  explicit NonterminalExpression(AbstractExpression *e) { expression = e; }

  void Interpreter(Context *context) override {
    std::cout << "NonterminalExpression: " << context->GetInput() << ", "
              << context->GetOutput() << std::endl;
    expression->Interpreter(context);
  }
};

} // end of namespace interpreter

TEST(interpreter, basic_demo) {
  using namespace interpreter;

  auto *c = new Context();
  c->SetInput("Hello");
  c->SetOutput("World");

  AbstractExpression *exp1 = new TerminalExpression();
  AbstractExpression *exp2 = new NonterminalExpression(exp1);

  exp1->Interpreter(c);
  exp2->Interpreter(c);

  delete exp1;
  delete exp2;
}

namespace calculator {

struct Token {
  enum Type { integer, plus, minus, lparen, rparen };
  Type type_;
  std::string text_;

  Token(Type type, std::string txt) : type_(type), text_(std::move(txt)) {}

  friend std::ostream &operator<<(std::ostream &os, const Token &o) {
    return os << "`" << o.text_ << "`";
  }
};

std::vector<Token> lex(const std::string &input) {
  std::vector<Token> res;

  for (auto curr = std::begin(input), end = std::end(input); curr != end;
       ++curr) {
    switch (*curr) {
    case '+':
      res.emplace_back(Token::plus, "+");
      break;
    case '-':
      res.emplace_back(Token::minus, "-");
      break;
    case '(':
      res.emplace_back(Token::lparen, "(");
      break;
    case ')':
      res.emplace_back(Token::rparen, ")");
      break;
    default:
      auto first_not_digit = std::find_if(curr, std::end(input),
          [](auto c) { return !std::isdigit(c); });
      std::string integer = std::string(curr, first_not_digit);
      res.emplace_back(Token::integer, integer);
      curr = --first_not_digit;
    }
  }

  return std::move(res);
}

} // end of namespace calculator

TEST(interpreter, calculator_demo) {
  using namespace calculator;

  auto tokens = lex("(13-4)-(12+1)");
  for (auto &t : tokens)
    std::cout << t << " ";
}