#pragma once

#include "ast_defines.inc"
#include "runtime_error.h"
#include "value.h"

namespace Lox {

class Interpreter final : public AstNodeVisitor {
public:
  void interpret(Expr *expr);

  [[nodiscard]] Value result() const { return m_result; }

  void visit(Literal &) override;

  void visit(Binary &) override;

  void visit(Unary &) override;

  void visit(Grouping &) override;

  ~Interpreter() noexcept override = default;

private:
  /**
   * @brief Evaluate the value of `expr`, and get the value using `result()`.
   */
  void evaluate(Expr *expr) { expr->accept(*this); }

  void check_number_operands(Token const &op, Value const &operand) {
    if (!operand.is_number()) {
      throw RuntimeError(op, "Operand must be a number.");
    }
  }

  void check_number_operands(Token const &op, Value const &left,
                             Value const &right) {
    if (!left.is_number() || !right.is_number()) {
      throw RuntimeError(op, "Operands must be numbers.");
    }
  }

private:
  Value m_result;
};

} // namespace Lox
