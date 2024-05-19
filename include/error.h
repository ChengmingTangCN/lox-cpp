#pragma once

#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>

namespace lox {
inline void error(int lineno, char const *msg) {
  std::cerr << lineno << ": "
            << "error: " << msg << '\n';
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
      throw lox::Exception(err_msg);                                           \
    }                                                                          \
  } while (false);
} // namespace lox
