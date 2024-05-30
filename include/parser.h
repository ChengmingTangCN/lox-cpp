#pragma once

#include "ast_defines.inc"
#include "scanner.h"

#include <error.h>
#include <initializer_list>

namespace Lox {

class ParseError : public Exception {
public:
  ParseError(char const *msg) : Exception(msg) {}

  ParseError(std::string msg) : Exception(std::move(msg)) {}

  ~ParseError() noexcept override = default;
};

class Parser {
public:
  Parser(std::vector<Token> const &tokens) : m_tokens(tokens), m_current() {}

  Parser(Parser const &) = delete;

  Parser &operator=(Parser const &) = delete;

  ~Parser() noexcept = default;

  ExprPtr parse();

private:
  ExprPtr expression();

  ExprPtr equality();

  ExprPtr comparison();

  ExprPtr term();

  ExprPtr factor();

  ExprPtr unary();

  ExprPtr primary();

private:
  /**
   * @brief
   */
  Token const &peek() noexcept { return m_tokens[m_current]; }

  /**
   * @brief
   */
  void advance() noexcept { ++m_current; }

  /**
   * @brief
   */
  bool match(std::initializer_list<TokenType> types) noexcept;

  /**
   * @brief
   */
  Token const &previous() noexcept { return m_tokens[m_current - 1]; }

  /**
   * @brief
   */
  void consume(std::initializer_list<TokenType> types,
               std::string_view error_msg);

  /**
   *
   */
  [[noreturn]] void error(Token const &token, std::string_view error_msg);

private:
  std::vector<Token> const &m_tokens;
  std::size_t m_current;
};

} // namespace Lox
