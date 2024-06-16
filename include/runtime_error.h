#pragma once

#include "error.h"
#include "scanner.h"

namespace Lox {

class RuntimeError : public Exception {
public:
  RuntimeError(Token const &token, std::string msg)
      : m_token(token), Exception("") {
    std::ostringstream out;
    out << "line " << m_token.lineno() << " : " << msg;
    m_msg = out.str();
  }

private:
  Token const &m_token;
};

extern std::vector<std::string> runtime_error_msgs;

/**
 * @brief Insert an error message into `runtime_error_msgs`, which can be dumped
 * later.
 */
inline void runtime_error(RuntimeError const &error) {
  std::ostringstream oss;
  if (!runtime_error_msgs.empty()) {
    oss << '\n';
  }
  oss << "error: " << error.what();
  runtime_error_msgs.emplace_back(std::move(oss).str());
}

} // namespace Lox
