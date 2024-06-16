#pragma once

#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace Lox {

extern std::vector<std::string> syntax_error_msgs;

/**
 * @brief Insert an error message into `syntax_error_msgs`, which can be dumped
 * later.
 */
inline void syntax_error(int lineno, char const *msg) {
  std::ostringstream oss;
  if (!syntax_error_msgs.empty()) {
    oss << '\n';
  }
  oss << "error: " << lineno << ": " << msg;
  syntax_error_msgs.emplace_back(std::move(oss).str());
}

/**
 * @brief Dump all messages from `msgs`. After this, `msgs` is empty.
 */
inline std::string dump_error_msgs(std::vector<std::string> &msgs) {
  std::ostringstream oss;
  for (auto &&msg : msgs) {
    oss << std::move(msg);
  }
  msgs.clear();
  return std::move(oss).str();
}

class Exception : public std::exception {
public:
  Exception(char const *msg) : m_msg(msg) {}

  Exception(std::string msg) : m_msg(std::move(msg)) {}

  virtual ~Exception() noexcept = default;

  char const *what() const noexcept override { return m_msg.c_str(); }

protected:
  std::string m_msg;
};

#define CHECK_ERRNO(rc, msg)                                                   \
  do {                                                                         \
    if (rc < 0) {                                                              \
      std::string err_msg{msg};                                                \
      err_msg += ": ";                                                         \
      err_msg += std::strerror(errno);                                         \
      throw Lox::Exception(err_msg);                                           \
    }                                                                          \
  } while (false);

#define THROW_ASSERT(pred, msg)                                                \
  do {                                                                         \
    if (!pred) {                                                               \
      throw Lox::Exception(msg);                                               \
    }                                                                          \
  } while (false);
} // namespace Lox
