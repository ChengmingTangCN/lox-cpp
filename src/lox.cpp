#include "ast_printer.h"
#include "file.h"
#include "interpreter.h"
#include "parser.h"
#include "runtime_error.h"
#include "scanner.h"

#include <cstdio>
#include <error.h>
#include <iostream>
#include <vector>

static void run(std::string const &source) {
  Lox::Scanner scanner(source);
  auto const &tokens = scanner.scan_tokens();

  Lox::Parser parser(tokens);
  Lox::ExprPtr expr = parser.parse();

  if (!Lox::syntax_error_msgs.empty()) {
    return;
  }

  for (auto const &token : tokens) {
    std::cout << token << '\n';
  }

  Lox::AstPrinter ast_printer(std::cout);
  expr->accept(ast_printer);
  std::cout << '\n';

  Lox::Interpreter interpreter;
  interpreter.interpret(expr.get());

  if (!Lox::runtime_error_msgs.empty()) {
    return;
  }

  std::cout << interpreter.result() << '\n';
}

static void run_file(char const *pathname) {
  auto const bytes = Lox::read_file(pathname);
  run(bytes);
  auto error_msg = Lox::dump_error_msgs(Lox::syntax_error_msgs);
  if (!error_msg.empty()) {
    throw Lox::Exception(std::move(error_msg));
  }
  error_msg = Lox::dump_error_msgs(Lox::runtime_error_msgs);
  if (!error_msg.empty()) {
    throw Lox::Exception(std::move(error_msg));
  }
}

static void run_prompt() {
  while (true) {
    std::cout << "> ";
    std::string line_str;
    if (!std::getline(std::cin, line_str)) {
      break;
    }
    run(line_str);
    auto error_msg = Lox::dump_error_msgs(Lox::syntax_error_msgs);
    if (!error_msg.empty()) {
      std::cerr << error_msg << std::endl;
    }
    error_msg = Lox::dump_error_msgs(Lox::runtime_error_msgs);
    if (!error_msg.empty()) {
      std::cerr << error_msg << std::endl;
    }
  }
}

int main(int argc, char *argv[]) {
  try {
    if (argc > 2) {
      std::cout << "Usage: " << argv[0] << " [*.lox]" << std::endl;
      return 1;
    } else if (argc == 2) {
      run_file(argv[1]);
    } else {
      run_prompt();
    }
  } catch (Lox::Exception const &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
