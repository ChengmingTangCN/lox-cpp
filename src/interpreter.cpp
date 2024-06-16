#include "interpreter.h"
#include "ast_defines.inc"
#include "error.h"
#include "runtime_error.h"
#include "scanner.h"

namespace Lox {

void Interpreter::interpret(Expr *expr) {
  try {
    evaluate(expr);
  } catch (RuntimeError const &e) {
    runtime_error(e);
  }
}

void Interpreter::visit(Literal &expr) {
  switch (expr.m_token.type()) {
  case TokenType::NUMBER:
    m_result = expr.m_token.number_literal();
    break;
  case TokenType::STRING:
    m_result = std::string(expr.m_token.str_literal());
    break;
  case TokenType::TRUE:
    m_result = true;
    break;
  case TokenType::FALSE:
    m_result = false;
    break;
  case TokenType::NIL:
    m_result = nullptr;
    break;
  default:
    THROW_ASSERT(false, "Literal must be number, string, boolean or nil.");
    break;
  }
}

void Interpreter::visit(Unary &expr) {
  evaluate(expr.m_right.get());

  switch (expr.m_op.type()) {
  case TokenType::MINUS:
    check_number_operands(expr.m_op, m_result);
    m_result = -m_result.number();
    break;
  case TokenType::BANG:
    m_result = !m_result.is_truthy();
    break;
  default:
    THROW_ASSERT(false, "Unimplemented unary operator: " +
                            std::string(expr.m_op.lexeme()));
    break;
  }
}

void Interpreter::visit(Binary &expr) {
  evaluate(expr.m_left.get());
  Value left = std::move(m_result);
  evaluate(expr.m_right.get());

  switch (expr.m_op.type()) {
  case TokenType::PLUS:
    if (left.is_number() && m_result.is_number()) {
      m_result = left.number() + m_result.number();
      break;
    } else if (left.is_string() && m_result.is_string()) {
      m_result = left.str() + m_result.str();
      break;
    }
    throw RuntimeError(expr.m_op, "Operands must be 2 numbers or strings.");
  case TokenType::MINUS:
    check_number_operands(expr.m_op, left, m_result);
    m_result = left.number() - m_result.number();
    break;
  case TokenType::STAR:
    check_number_operands(expr.m_op, left, m_result);
    m_result = left.number() * m_result.number();
    break;
  case TokenType::SLASH:
    check_number_operands(expr.m_op, left, m_result);
    m_result = left.number() / m_result.number();
    break;
  case TokenType::GREATER:
    check_number_operands(expr.m_op, left, m_result);
    m_result = left.number() > m_result.number();
    break;
  case TokenType::GREATER_EQUAL:
    check_number_operands(expr.m_op, left, m_result);
    m_result = left.number() >= m_result.number();
    break;
  case TokenType::LESS:
    check_number_operands(expr.m_op, left, m_result);
    m_result = left.number() < m_result.number();
    break;
  case TokenType::LESS_EQUAL:
    check_number_operands(expr.m_op, left, m_result);
    m_result = left.number() <= m_result.number();
    break;
  case TokenType::EQUAL_EQUAL:
    m_result = left == m_result;
    break;
  case TokenType::BANG_EQUAL:
    m_result = left != m_result;
    break;
  default:
    THROW_ASSERT(false, "Unimplemented binary operator: " +
                            std::string(expr.m_op.lexeme()));
    break;
  }
}

void Interpreter::visit(Grouping &expr) { evaluate(expr.m_expr.get()); }

} // namespace Lox
