#include "ast_printer.h"
#include "scanner.h"

namespace Lox {

void AstPrinter::visit(Binary &node) {
  m_out << node.m_op.lexeme();
  m_out << " ";
  node.m_left->accept(*this);
  m_out << " ";
  node.m_right->accept(*this);
}

void AstPrinter::visit(Unary &node) {
  m_out << node.m_op.lexeme();
  m_out << " ";
  node.m_right->accept(*this);
}

void AstPrinter::visit(Grouping &node) {
  m_out << "(";
  node.m_expr->accept(*this);
  m_out << ")";
}

void AstPrinter::visit(Literal &node) {
  switch (node.m_token.type()) {
  case TokenType::TRUE:
    m_out << "true";
    break;
  case TokenType::FALSE:
    m_out << "false";
    break;
  case TokenType::NIL:
    m_out << "nil";
    break;
  case TokenType::NUMBER:
    m_out << node.m_token.number_literal();
    break;
  case TokenType::STRING:
    m_out << node.m_token.str_literal();
    break;
  default:
    Lox::error(
        node.m_token.lineno(),
        "An error occurred in Lox. A literal of invalid type was encountered.");
    break;
  }
}

} // namespace Lox
