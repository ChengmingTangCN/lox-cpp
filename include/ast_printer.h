#pragma once

#include "ast_defines.inc"

namespace Lox {

class AstPrinter final : public AstNodeVisitor {
public:
  AstPrinter(std::ostream &out) : m_out(out) {}

  AstPrinter(AstPrinter const &) = delete;

  AstPrinter(AstPrinter &&) = delete;

  AstPrinter &operator=(AstPrinter const &) = delete;

  AstPrinter &operator=(AstPrinter &&) = delete;

  ~AstPrinter() noexcept override = default;

  void visit(Binary &node) override;

  void visit(Unary &node) override;

  void visit(Grouping &node) override;

  void visit(Literal &node) override;

private:
  std::ostream &m_out;
};

} // namespace Lox
