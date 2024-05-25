#pragma once

#include "error.h"

#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Lox {

enum class TokenType : uint8_t {
  // single-character tokens
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  PLUS,
  MINUS,
  SEMICOLON,
  SLASH,
  STAR,

  // one or two character tokens
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  LESS,
  LESS_EQUAL,
  GREATER,
  GREATER_EQUAL,

  IDENTIFIER,
  STRING,
  NUMBER,

  // keywords
  VAR,
  TRUE,
  FALSE,
  NIL,
  FUN,
  RETURN,
  CLASS,
  THIS,
  SUPER,
  AND,
  OR,
  IF,
  ELSE,
  WHILE,
  FOR,
  PRINT,

  END
};

inline std::string to_string(TokenType type) {
  switch (type) {
  // single-character tokens
  case TokenType::LEFT_PAREN:
    return "LEFT_PAREN";
  case TokenType::RIGHT_PAREN:
    return "RIGHT_PAREN";
  case TokenType::LEFT_BRACE:
    return "LEFT_BRACE";
  case TokenType::RIGHT_BRACE:
    return "RIGHT_BRACE";
  case TokenType::COMMA:
    return "COMMA";
  case TokenType::DOT:
    return "DOT";
  case TokenType::PLUS:
    return "PLUS";
  case TokenType::MINUS:
    return "MINUS";
  case TokenType::SEMICOLON:
    return "SEMICOLON";
  case TokenType::SLASH:
    return "SLASH";
  case TokenType::STAR:
    return "STAR";
  case TokenType::BANG:
    return "BANG";
  case TokenType::BANG_EQUAL:
    return "BANG_EQUAL";
  case TokenType::EQUAL:
    return "EQUAL";
  case TokenType::EQUAL_EQUAL:
    return "EQUAL_EQUAL";
  case TokenType::LESS:
    return "LESS";
  case TokenType::LESS_EQUAL:
    return "LESS_EQUAL";
  case TokenType::GREATER:
    return "GREATER";
  case TokenType::GREATER_EQUAL:
    return "GREATER_EQUAL";
  case TokenType::IDENTIFIER:
    return "IDENTIFIER";
  case TokenType::STRING:
    return "STRING";
  case TokenType::NUMBER:
    return "NUMBER";
  case TokenType::VAR:
    return "VAR";
  case TokenType::TRUE:
    return "TRUE";
  case TokenType::FALSE:
    return "FALSE";
  case TokenType::NIL:
    return "NIL";
  case TokenType::FUN:
    return "FUN";
  case TokenType::RETURN:
    return "RETURN";
  case TokenType::CLASS:
    return "CLASS";
  case TokenType::THIS:
    return "THIS";
  case TokenType::SUPER:
    return "SUPER";
  case TokenType::AND:
    return "AND";
  case TokenType::OR:
    return "OR";
  case TokenType::IF:
    return "IF";
  case TokenType::ELSE:
    return "ELSE";
  case TokenType::WHILE:
    return "WHILE";
  case TokenType::FOR:
    return "FOR";
  case TokenType::PRINT:
    return "PRINT";
  case TokenType::END:
    return "END";
  default:
    return "???";
  }
}

class Token {
  friend std::ostream &operator<<(std::ostream &out, Token const &token);

public:
  Token(uint32_t lineno, TokenType type, std::string lexeme);

  Token(Token const &other);

  Token &operator=(Token const &other);

  Token(Token &&) noexcept;

  Token &operator=(Token &&) noexcept;

  ~Token() noexcept;

  void swap(Token &other) noexcept;

private:
  uint32_t m_lineno;
  TokenType m_type;
  std::string m_lexeme;
  union {
    char *str;
    double number;
    uint64_t data;
  } m_literal;
};

inline void swap(Token &lhs, Token &rhs) noexcept { lhs.swap(rhs); }

class Scanner {
public:
  Scanner(std::string const &source) : m_source(source) {}

  /**
   * @brief Scan out all tokens in the source. Need to find all errors possible.
   */
  std::vector<Token> const &scan_tokens();

private:
  /**
   * @brief Scan a token from the left characters.
   */
  void scan_token();

  /**
   * @brief if no characters left, return `true`, otherwise return `false`.
   */
  bool is_at_end() { return m_current >= m_source.size(); }

  /**
   * @brief Consume a character and return it.
   */
  [[nodiscard]] char advance() noexcept { return m_source[m_current++]; }

  /**
   * @brief If the current character is equal to `expected`, consume it
   *        and return `true`, otherwise return `false` directly.
   */
  [[nodiscard]] bool match(char expected) noexcept {
    if (is_at_end())
      return false;
    if (expected == m_source[m_current]) {
      ++m_current;
      return true;
    }
    return false;
  }

  template <typename Predicate>
  bool match(Predicate const &predicate) {
    if (is_at_end())
      return false;
    if (predicate(m_source[m_current])) {
      ++m_current;
      return true;
    }
    return false;
  }

  /**
   * @brief Returns the current character, but does not consume it.
   */
  [[nodiscard]] char peek() noexcept {
    if (is_at_end()) {
      return '\0';
    }
    return m_source[m_current];
  }

  /**
   * @brief Return the next character, but does not consume the
   *        current character and it.
   */
  [[nodiscard]] char peek_next() noexcept {
    if (m_current + 1 >= m_source.size()) {
      return '\0';
    }
    return m_source[m_current + 1];
  }

  /**
   * @brief Skip the current character.
   */
  void skip() noexcept { ++m_current; }

  /**
   * @brief Construct a token from [`m_start`, `m_current`) and put it
   *        into `m_tokens`.
   */
  void add_token(TokenType type) {
    m_tokens.emplace_back(m_lineno, type,
                          std::string(m_source, m_start, m_current - m_start));
  }

  /**
   * @brief Construct a token from `lexeme` and put it into `m_tokens`.
   */
  void add_token(TokenType type, std::string lexeme) {
    m_tokens.emplace_back(m_lineno, type, std::move(lexeme));
  }

  [[nodiscard]] static bool is_digit(char c) { return c >= '0' && c <= '9'; }

  [[nodiscard]] static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
  }

  [[nodiscard]] static bool is_alpha_numeric(char c) {
    return is_alpha(c) || is_digit(c);
  }

  /**
   * @brief Consume characters to construct a string literal, and end
   *        the traversal when encountering ".
   */
  void tokenize_string();

  /**
   * @brief Consume characters to construct a number literal.
   *
   * All number in Lox are floating point at runtime, but both integers and
   * decimal literals are supported.
   *
   * Lox does not support a leading or trailing decimal point.
   */
  void tokenize_number();

  /**
   * @brief Consume characters to construct an identifier or keyword.
   */
  void tokenize_identifier();

private:
  std::string const &m_source;
  std::vector<Token> m_tokens{};

  static std::unordered_map<std::string, TokenType> keywords;

  uint32_t m_lineno = 1;
  uint64_t m_current{};
  uint64_t m_start{};
};

} // namespace Lox