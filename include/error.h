#pragma once

#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace Lox {

extern std::vector<std::string> error_msgs;

/**
 * @brief Insert an error message into `error_msgs`, which can be dumped later.
 */
inline void error(int lineno, char const *msg) {
  std::ostringstream oss;
  if (!error_msgs.empty()) {
    oss << '\n';
  }
  oss << "error: " << lineno << ": " << msg;
  error_msgs.emplace_back(std::move(oss).str());
}

/**
 * @brief Dump all messages from `error_msgs`. After this, `error_msgs` is
 * empty.
 */
inline std::string dump_error_msgs() {
  std::ostringstream oss;
  for (auto &&msg : error_msgs) {
    oss << std::move(msg);
  }
  error_msgs.clear();
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
} // namespace Lox
