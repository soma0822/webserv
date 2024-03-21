#ifndef WEVSERB_SRC_UTILS_OPTION_HPP
#define WEVSERB_SRC_UTILS_OPTION_HPP

#include <stdexcept>

template <typename T>
class Option {
 public:
  Option() {}
  Option(const Option &obj) { (void)obj; }
  virtual ~Option() {}
  Option &operator=(const Option &obj) {
    (void)obj;
    return *this;
  }

  virtual T Unwrap() = 0;
  virtual bool IsSome() = 0;
};

template <typename T>
class None : public Option<T> {
 public:
  None() : Option<T>() {}
  None(const Option<T> &obj) : Option<T>(obj) {}
  ~None() {}
  None &operator=(const None<T> &obj) {
    Option<T>::operator=(obj);
    return *this;
  }

  T Unwrap() { throw std::runtime_error("Invalid value"); }
  bool IsSome() { return false; }
};

template <typename T>
class Some : public Option<T> {
 public:
  Some() {}
  Some(T value) : Option<T>(), value_(value) {}
  Some(const Some<T> &obj) : Option<T>(obj), value_(obj.value_) {}
  Some &operator=(const Some<T> &obj) {
    value_ = obj.value_;
    return *this;
  }

  T Unwrap() { return value_; }
  bool IsSome() { return true; }

 private:
  T value_;
};

#endif  // WEVSERB_SRC_UTILS_OPTION_HPP
