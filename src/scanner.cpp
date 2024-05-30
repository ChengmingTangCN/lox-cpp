#include "scanner.h"

#include "error.h"
#include <unordered_map>

namespace Lox {

std::ostream &operator<<(std::ostream &out, Token const &token) {
  out << to_string(token.m_type) << " "
      << "\"" << token.m_lexeme << "\"";

  if (token.m_type == TokenType::STRING) {
    out << " \"" << token.m_literal.str << "\"";
  } else if (token.m_type == TokenType::NUMBER) {
    out << std::fixed << " " << token.m_literal.number;
  }
  return out;
}

void Token::swap(Token &other) noexcept {
  std::swap(m_lineno, other.m_lineno);
  std::swap(m_type, other.m_type);
  m_lexeme.swap(other.m_lexeme);
  std::swap(m_literal, other.m_literal);
}

Token::Token(uint32_t lineno, TokenType type, std::string lexeme)
    : m_lineno(lineno), m_type(type), m_lexeme(std::move(lexeme)) {
  if (type == TokenType::STRING) {
    // Initialize string literal
    // The head and tail of m_lexeme are "
    auto const len = m_lexeme.size() - 2;
    m_literal.str = new char[len + 1];
    std::memcpy(m_literal.str, m_lexeme.c_str() + 1, len);
    m_literal.str[len] = '\0';
  } else if (type == TokenType::NUMBER) {
    // Initialize number literal
    double number = std::stod(m_lexeme);
    m_literal.number = number;
  }
}

Token::Token(Token const &other)
    : Token(other.m_lineno, other.m_type, other.m_lexeme) {
  if (other.m_type == TokenType::STRING) {
    auto const str_len = std::strlen(m_literal.str);
    char *new_str = new char[str_len + 1];
    std::memcpy(new_str, other.m_literal.str, str_len + 1);
    m_literal.str = new_str;
  } else {
    m_literal.data = other.m_literal.data;
  }
}

Token::Token(Token &&other) noexcept {
  m_lineno = other.m_lineno;
  m_type = other.m_type;
  m_lexeme = std::move(other.m_lexeme);
  m_literal = other.m_literal;

  other.m_type = TokenType::END;
}

Token &Token::operator=(Token const &other) {
  Token tmp{other};
  tmp.swap(*this);

  return *this;
}

Token &Token::operator=(Token &&other) noexcept {
  Token tmp{std::move(other)};
  tmp.swap(*this);

  return *this;
}

Token::~Token() noexcept {
  if (m_type == TokenType::STRING) {
    delete[] m_literal.str;
  }
}

std::unordered_map<std::string, TokenType> Scanner::keywords = {
    {"var", TokenType::VAR},     {"true", TokenType::TRUE},
    {"false", TokenType::FALSE}, {"nil", TokenType::NIL},
    {"fun", TokenType::FUN},     {"return", TokenType::RETURN},
    {"class", TokenType::CLASS}, {"this", TokenType::THIS},
    {"super", TokenType::SUPER}, {"and", TokenType::AND},
    {"or", TokenType::OR},       {"if", TokenType::IF},
    {"else", TokenType::ELSE},   {"while", TokenType::WHILE},
    {"for", TokenType::FOR},     {"print", TokenType::PRINT},
};

void Scanner::tokenize_string() {
  while (!is_at_end()) {
    if (match('"')) {
      add_token(TokenType::STRING);
      return;
    } else if (match('\n')) {
      // Lox supports multi-line string literals
      ++m_lineno;
    } else {
      skip();
    }
  }
  Lox::error(m_lineno, "Unterminated string literal");
}

void Scanner::tokenize_number() {
  auto skip_digits = [this] {
    while (match(is_digit)) {
      // nop
    }
  };

  skip_digits();

  if (peek() == '.' && is_digit(peek_next())) {
    // The decimal point
    skip();
    skip_digits();
  }

  add_token(TokenType::NUMBER);
}

void Scanner::tokenize_identifier() {
  while (match(is_alpha_numeric)) {
    // nop
  }

  std::string identifier(m_source, m_start, m_current - m_start);

  auto iter = keywords.find(identifier);
  if (iter != keywords.end()) {
    add_token(iter->second, std::move(identifier));
  } else {
    add_token(TokenType::IDENTIFIER, std::move(identifier));
  }
}
std::vector<Token> const &Scanner::scan_tokens() {
  while (!is_at_end()) {
    m_start = m_current;
    scan_token();
  }
  add_token(TokenType::END, "");
  return m_tokens;
}

void Scanner::scan_token() {
  char const c = advance();
  switch (c) {
  case '(':
    add_token(TokenType::LEFT_PAREN);
    break;
  case ')':
    add_token(TokenType::RIGHT_PAREN);
    break;
  case '{':
    add_token(TokenType::LEFT_BRACE);
    break;
  case '}':
    add_token(TokenType::RIGHT_BRACE);
    break;
  case ',':
    add_token(TokenType::COMMA);
    break;
  case '.':
    add_token(TokenType::DOT);
    break;
  case '+':
    add_token(TokenType::PLUS);
    break;
  case '-':
    add_token(TokenType::MINUS);
    break;
  case ';':
    add_token(TokenType::SEMICOLON);
    break;
  case '/':
    if (match('/')) {
      while (!is_at_end() && peek() != '\n') {
        skip();
      }
    } else {
      add_token(TokenType::SLASH);
    }
    break;
  case '*':
    add_token(TokenType::STAR);
    break;
  case '!':
    match('=') ? add_token(TokenType::BANG_EQUAL) : add_token(TokenType::BANG);
    break;
  case '=':
    match('=') ? add_token(TokenType::EQUAL_EQUAL)
               : add_token(TokenType::EQUAL);
    break;
  case '<':
    match('=') ? add_token(TokenType::LESS_EQUAL) : add_token(TokenType::LESS);
    break;
  case '>':
    match('=') ? add_token(TokenType::GREATER_EQUAL)
               : add_token(TokenType::GREATER_EQUAL);
    break;
  case '"':
    tokenize_string();
    break;
  case ' ':
  case '\r':
  case '\t':
    // Ignore whitespaces
    break;
  case '\n':
    ++m_lineno;
    break;
  default:
    if (is_digit(c)) {
      tokenize_number();
      break;
    } else if (is_alpha(c)) {
      tokenize_identifier();
      break;
    }
    error(m_lineno, "Invalid character");
    break;
  }
}

} // namespace Lox
