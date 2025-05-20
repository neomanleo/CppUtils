#ifndef UTILS_STRING_HPP
#define UTILS_STRING_HPP

#include "macros.hpp"
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <sys/types.h>

class UtilsSimpleString {
public:
  UtilsSimpleString(const char *string) {
    if (string == nullptr) {
      throw std::invalid_argument(
          "UtilsSimpleString() constructor - passed in nullptr");
    }

    length = std::strlen(string) + 1;
    capacity = length;
    ALIGN_MOD8(capacity);

    r_data = new char[capacity];
    std::memcpy(r_data, string, length);

    std::cout << "CONSTRUCTED\n" << std::endl;
  };
  UtilsSimpleString() {
    length = 0;
    capacity = 8;

    r_data = new char[capacity];
    std::cout << "DEFAULT CONSTRUCTED\n" << std::endl;
  };
  UtilsSimpleString(const UtilsSimpleString &other) {
    length = other.length;
    capacity = other.capacity;
    r_data = new char[capacity];
    std::memcpy(r_data, other.r_data, capacity);
    std::cout << "COPY CONSTRUCTED\n" << std::endl;
  };

  UtilsSimpleString(UtilsSimpleString &&other) {
    capacity = other.capacity;
    length = other.length;
    r_data = other.r_data;

    other.capacity = 0;
    other.length = 0;
    other.r_data = nullptr;
    std::cout << "MOVE CONSTRUCTED\n" << std::endl;
  };

  ~UtilsSimpleString() { delete[] r_data; };

  char *data() { return r_data; }

  char *c_str() {
    if (r_data[length] != '\0') {
      r_data[length] = '\0';
    }
    return r_data;
  };

  char &at(size_t index) {
    if (index <= length) {
      return r_data[index];
    }
    throw std::out_of_range(".at() - out of bounds index.");
  };

  void push_back(char c) {
    if ((length + 1) >= capacity) {
      resize(64);
    }
    if (length > 0) {
      --length;
      r_data[length] = c;
      ++length;
      r_data[length] = '\0';
      ++length;
    } else {
      r_data[length] = c;
      ++length;
      r_data[length] = '\0';
      ++length;
    }
  };

  size_t size() { return length; };

  UtilsSimpleString substr(size_t index, size_t index_offset) {
    if ((index >= length) || (index_offset >= length)) {
      throw std::out_of_range(".substr() - out of bounds index.");
    }

    UtilsSimpleString str;
    for (size_t i = index; i <= (index_offset + index); ++i) {
      str.push_back(r_data[i]);
    }

    return str;
  };

  void erase(size_t index) {
    if (index < length) {
      r_data[index] = '\0';
      char *tmp = new char[capacity];
      for (size_t i = 0, j = 0; i < length; ++i) {
        if (r_data[i] != '\0') {
          tmp[j] = r_data[i];
          ++j;
        }
      }
      delete[] r_data;
      r_data = tmp;
      --length;
      r_data[length] = '\0';
    } else {
      throw std::out_of_range(".erase() - out of bounds index.");
    }
  };

  void append(const char *string) {
    if (string == nullptr) {
      throw std::invalid_argument(".append() - passed in nullptr");
    }
    for (size_t i = 0; i < std::strlen(string); ++i) {
      push_back(string[i]);
    }
  };

  friend std::ostream &operator<<(std::ostream &stream,
                                  UtilsSimpleString &string) {
    stream << string.c_str();
    return stream;
  };

private:
  size_t capacity;
  size_t length; // will always be the null terminator
  char *r_data;

  void resize(ssize_t change) {
    if (change > 0) {
      char *tmp = new char[capacity + static_cast<size_t>(change)];
      std::memcpy(tmp, r_data, capacity);

      delete[] r_data;

      r_data = tmp;
      capacity += static_cast<size_t>(change);
    } else {
      capacity += static_cast<size_t>(change);

      char *tmp = new char[capacity];

      std::memcpy(tmp, r_data, capacity);

      delete[] r_data;
      r_data = tmp;
    }
  };
};
#endif
