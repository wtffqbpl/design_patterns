#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct Token {
  enum Type {integer, plus, minus, lparen, rparen};
  Type m_type;
  std::string m_text;

  Token(Type type, const std::string &txt)
    : m_type(type), m_text(txt) {}

  friend std::ostream& operator<<(std::ostream &os, const Token &o) {
    return os << "`" << o.m_text << "`";
  }
};

std::vector<Token> lex(const std::string &input) {
  std::vector<Token> res;

  for (auto curr = std::begin(input); curr != std::end(input); ++curr) {
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
        auto first_not_digit = std::find_if(curr, std::end(input), [](auto c) {
            return !std::isdigit(c);
            });
        std::string integer = std::string(curr, first_not_digit);
        res.emplace_back(Token::integer, integer);
        curr = --first_not_digit;
    }
  }

  return res;
}

int main() {
  auto tokens = lex("(13-4)-(12+1)");

  for (auto &t : tokens) {
    std::cout << t << " ";
  }

  return 0;
}
