#include "value.h"
#include "error.h"

#include <cstring>

namespace Lox {

bool operator==(Value const &lhs, Value const &rhs) {
  return lhs.m_data == rhs.m_data;
}

bool operator!=(Value const &lhs, Value const &rhs) { return !(lhs == rhs); }

std::ostream &operator<<(std::ostream &out, Value const &val) {
  if (val.is_nil()) {
    out << "nil";
  } else if (val.is_number()) {
    out << val.number();
  } else if (val.is_string()) {
    out << val.str();
  } else if (val.is_boolean()) {
    out << (val.boolean() ? "true" : "false");
  } else {
    out << "none";
  }
  return out;
}

} // namespace Lox
