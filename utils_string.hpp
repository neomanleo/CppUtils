#ifndef UTILS_STRING_HPP
#define UTILS_STRING_HPP
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include "macros.hpp"
// WIP

#define LEN_TO_STR std::to_string(current_length)
class UtilsString {

public:
  UtilsString() {
    current_length = 0;
    capacity = (current_length + 1);

    ALIGN_MOD8(capacity);
    raw_data = new char[capacity];

    std::memset(raw_data, 0, current_length);
  };

  UtilsString(const char *input_str) {
    current_length = strlen(input_str);
    capacity = (current_length + 1);
    ALIGN_MOD8(capacity);
    raw_data = new char[capacity];

    std::memset(raw_data, 0, current_length);
    std::memcpy(raw_data, input_str, current_length);
  };

  UtilsString(UtilsString &other) {
    current_length = other.current_length;
    capacity = other.capacity;

    raw_data = new char[capacity];
    memcpy(raw_data, other.raw_data, other.capacity);
  };

  ~UtilsString() { delete[] raw_data; };

  char *data() { return raw_data; };

  char &at(size_t pos) {
    if (pos < current_length) {
      return raw_data[pos];
    } else {
      throw std::out_of_range(".at() pos is " + std::to_string(pos) +
                              " where length is " + LEN_TO_STR);
    }
  };

  char *c_str() {
    if (raw_data[current_length] != '\0') {
      push_back('\0');
      return data();
    }
    return data();
  };

  void push_back(char c) {
    if ((current_length + 1) < capacity) {
      ++current_length;
      raw_data[current_length] = c;
    } else {
      grow(1);
      raw_data[current_length + 1] = c;
      ++current_length;
    }
  };

  UtilsString substr(size_t idx = 0, size_t size = 1) {
    if ((idx >= current_length) || (size >= current_length) ||
        ((idx + size) >= current_length)) {
      throw std::out_of_range(".substr(), idx or size is >= " + LEN_TO_STR);
    }

    UtilsString ret;
    for (size_t i = idx; i < (idx + size); ++i) {
      ret.push_back(at(i));
    }
    return ret;
  };

  std::ostream &write(std::ostream &os) const
{
    return os;
  };

private:
  size_t current_length;
  size_t capacity;
  char *raw_data;

  void grow(size_t minimum_add) {
    size_t old_capacity = capacity;
    capacity += minimum_add;

    ALIGN_MOD8(capacity);

    char *realloc_raw_data = new char[capacity];
    memset(realloc_raw_data, 0, capacity);
    memcpy(realloc_raw_data, raw_data, old_capacity);

    delete[] raw_data;
    raw_data = realloc_raw_data;
  }
};
// std::ostream &operator<<(std::ostream &os, UtilsString const &othre) {
//   // return os << other
// };
#endif
