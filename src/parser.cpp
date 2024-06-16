#include "parser.h"
#include "scanner.h"

namespace Lox {
ExprPtr Parser::parse() {
  try {
    ExprPtr expr = expression();
    return expr;
  } catch (ParseError) {
    return nullptr;
  }
}

ExprPtr Parser::expression() { return equality(); }

// equality = comparison (( "==" | "!=" ) comparison )*
ExprPtr Parser::equality() {
  ExprPtr ans = comparison();

  while (match({TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL})) {
    auto const &op = previous();
    ans = std::make_unique<Binary>(std::move(ans), op, comparison());
  }

  return ans;
}

ExprPtr Parser::comparison() {
  ExprPtr ans = term();

  while (match({TokenType::LESS, TokenType::LESS_EQUAL, TokenType::GREATER,
                TokenType::GREATER_EQUAL})) {
    auto const &op = previous();
    ans = std::make_unique<Binary>(std::move(ans), op, term());
  }

  return ans;
}

ExprPtr Parser::term() {
  ExprPtr ans = factor();

  while (match({TokenType::PLUS, TokenType::MINUS})) {
    auto const &op = previous();
    ans = std::make_unique<Binary>(std::move(ans), op, factor());
  }

  return ans;
}

ExprPtr Parser::factor() {
  ExprPtr ans = unary();

  while (match({TokenType::STAR, TokenType::SLASH})) {
    auto const &op = previous();
    ans = std::make_unique<Binary>(std::move(ans), op, unary());
  }

  return ans;
}

ExprPtr Parser::unary() {
  if (match({TokenType::BANG, TokenType::MINUS})) {
    auto const &op = previous();
    return std::make_unique<Unary>(op, unary());
  }

  return primary();
}

ExprPtr Parser::primary() {
  if (match({TokenType::NUMBER, TokenType::STRING, TokenType::TRUE,
             TokenType::FALSE, TokenType::NIL})) {
    return std::make_unique<Literal>(previous());
  }

  if (match({TokenType::LEFT_PAREN})) {
    ExprPtr ans = expression();
    consume({TokenType::RIGHT_PAREN}, "Expect ')' after expression.");

    return std::make_unique<Grouping>(std::move(ans));
  }

  error(peek(), "Expect expression.");
}

bool Parser::match(std::initializer_list<TokenType> types) noexcept {
  for (auto type : types) {
    if (type == peek().m_type) {
      advance();
      return true;
    }
  }

  return false;
}

void Parser::consume(std::initializer_list<TokenType> types,
                     std::string_view error_msg) {
  for (auto type : types) {
    if (type == peek().m_type) {
      advance();
      return;
    }
  }

  error(peek(), error_msg);
}

[[noreturn]] void Parser::error(Token const &token,
                                std::string_view error_msg) {
  std::string msg;
  if (token.m_type == TokenType::END) {
    msg = "at end: " + std::string(error_msg);
  } else {
    msg = token.m_lexeme + ": " + std::string(error_msg);
  }
  Lox::syntax_error(token.m_lineno, msg.c_str());
  throw ParseError("parse error");
}

}; // namespace Lox
