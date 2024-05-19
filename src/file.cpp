#include "file.h"

#include "error.h"

#include <cstdio>
#include <string>

namespace lox {

std::string read_file(char const *pathname) {
  std::string buffer;
  FILE *fp = std::fopen(pathname, "r");
  if (!fp) {
    CHECK_ERRNO(-1, "fopen");
  }

  CHECK_ERRNO(std::fseek(fp, 0, SEEK_END), "fseek");
  auto const file_sz = ::ftell(fp);
  CHECK_ERRNO(std::fseek(fp, 0, SEEK_SET), "fseek");
  buffer.resize(file_sz);

  auto left_len = file_sz;
  while (left_len > 0) {
    auto const read_len = std::fread(buffer.data(), 1, left_len, fp);
    left_len -= read_len;
    if (std::ferror(fp)) {
      throw lox::Exception("fread failed");
    }
  }

  return buffer;
}
} // namespace lox