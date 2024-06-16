#pragma once

#include <iostream>
#include <string>
#include <variant>

namespace Lox {

/**
 * A static type is required to hold the result of the Lox expression
 * evaluation, which can be Number, String, Boolean.
 */
struct Value {
  friend bool operator==(Value const &lhs, Value const &rhs);
  friend bool operator!=(Value const &lhs, Value const &rhs);
  friend std::ostream &operator<<(std::ostream &out, Value const &val);

public:
  Value() = default;

  Value(double number) : m_data(number) {}

  Value(std::string str) : m_data(std::move(str)) {}

  Value(bool boolean) : m_data(boolean) {}

  Value(std::nullptr_t) : m_data(nullptr) {}

  Value(Value const &other) : m_data(other.m_data) {}

  Value(Value &&other) : m_data(std::move(other.m_data)) {}

  Value &operator=(Value const &other) {
    Value tmp{other};
    tmp.swap(*this);
    return *this;
  }

  Value &operator=(Value &&other) {
    Value tmp{std::move(other)};
    tmp.swap(*this);
    return *this;
  }

  void swap(Value &other) { std::swap(m_data, other.m_data); }

  ~Value() noexcept = default;

  bool is_number() const { return std::holds_alternative<double>(m_data); }

  bool is_string() const { return std::holds_alternative<std::string>(m_data); }

  bool is_boolean() const { return std::holds_alternative<bool>(m_data); }

  bool is_nil() const { return std::holds_alternative<std::nullptr_t>(m_data); }

  /**
   * In Lox, `false` and `nil` are falsey, and everything else is truthy.
   */
  bool is_truthy() const { return !(is_nil() || (is_boolean() && !boolean())); }

  double number() const { return std::get<double>(m_data); };

  bool boolean() const { return std::get<bool>(m_data); };

  std::string const &str() const { return std::get<std::string>(m_data); };

private:
  std::variant<double, bool, std::string, std::nullptr_t> m_data;
};

inline void swap(Value &lhs, Value &rhs) { lhs.swap(rhs); }

bool operator==(Value const &lhs, Value const &rhs);

bool operator!=(Value const &lhs, Value const &rhs);

std::ostream &operator<<(std::ostream &out, Value const &val);

} // namespace Lox
