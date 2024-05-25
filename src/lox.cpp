#include "file.h"
#include "scanner.h"

#include <cstdio>
#include <error.h>
#include <iostream>
#include <vector>

static void run(std::string const &source) {
  Lox::Scanner scanner(source);
  auto const &tokens = scanner.scan_tokens();
  for (auto const &token : tokens) {
    std::cout << token << '\n';
  }
}

static void run_file(char const *pathname) {
  auto const bytes = Lox::read_file(pathname);
  run(bytes);
  auto error_msg = Lox::dump_error_msgs();
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
    auto error_msg = Lox::dump_error_msgs();
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