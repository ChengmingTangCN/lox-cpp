#include "file.h"

#include <error.h>
#include <iostream>
#include <vector>

#include <sysexits.h>

static void run(std::string const &source) {}

static void run_file(char const *pathname) {
  auto const bytes = lox::read_file(pathname);
  run(bytes);
}

static void run_prompt() {
  while (true) {
    std::cout << "> ";
    std::string line_str;
    if (!std::getline(std::cin, line_str)) {
      break;
    }
    run(line_str);
  }
}

int main(int argc, char *argv[]) {
  try {
    if (argc > 2) {
      std::cout << "Usage: " << argv[0] << " [*.lox]" << std::endl;
      return EX_USAGE;
    } else if (argc == 2) {
      run_file(argv[1]);
    } else {
      run_prompt();
    }
  } catch (lox::Exception const &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}