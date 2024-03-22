#ifndef WEVSERB_SRC_UTILS_OPTION_HPP
#define WEVSERB_SRC_UTILS_OPTION_HPP

#include <stdexcept>

template <typename T>
class Option {
 public:
  Option(T value, bool has_value) : value_(value), has_value_(has_value) {}
  Option(const Option &obj): value_(obj.value_), has_value_(obj.has_value_) {}
  virtual ~Option() {}
  Option &operator=(const Option &obj) {
    if (this == &obj) {
      return *this;
    }
    value_ = obj.value_;
    has_value_ = obj.has_value_;
    return *this;
  }

  T Unwrap() {
    if (has_value_) {
      return value_;
    } else {
      throw std::runtime_error("Option does not contain a value");
    }
  }

  bool IsSome() { return has_value_; }

 private:
  T value_;
  bool has_value_;
};

template <typename T>
Option<T> None() {
  return Option<T>(T(), false);
}

template <typename T>
Option<T> Some(T value) {
  return Option<T>(value, true);
}

#endif  // WEVSERB_SRC_UTILS_OPTION_HPP
