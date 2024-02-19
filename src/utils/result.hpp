#ifndef WEBSERV_SRC_UTILS_RESULT_HPP
#define WEBSERV_SRC_UTILS_RESULT_HPP

#include <stdexcept>

/*
 * RustライクなResultクラス
 * 値とエラーのどちらか一方を持つ
 */

template <typename OkT, typename ErrT> class Result {
public:
  Result(OkT value, ErrT error, bool has_value)
      : value_(value), error_(error), has_value_(has_value) {}

  Result(const Result &other)
      : value_(other.value_), error_(other.error_),
        has_value_(other.has_value_) {}

  Result &operator=(const Result &other) {
    if (this == &other) {
      return *this;
    }
    value_ = other.value_;
    error_ = other.error_;
    has_value_ = other.has_value_;
  }

  ~Result() {}

  bool operator<(const Result &other) const {
    if (has_value_) {
      return value_ < other.value_;
    } else {
      return error_ < other.error_;
    }
  }

  // Errに対してUnwrapを呼ぶと例外を投げる
  OkT Unwrap() const {
    if (!IsOk()) {
      throw std::runtime_error("Result does not have a value");
    }
    return value_;
  }

  // Okに対してUnwrapErrを呼ぶと例外を投げる
  ErrT UnwrapErr() const {
    if (!IsErr()) {
      throw std::runtime_error("Result does not have an error");
    }
    return error_;
  }

  bool IsOk() const { return has_value_; }

  bool IsErr() const { return !has_value_; }

private:
  OkT value_;
  ErrT error_;
  bool has_value_;
};

namespace details {

/*
 * ValueとErrorクラスはResultクラスに変換するためのキャスト演算子を持つ
 * これによってResultクラスを生成するためのヘルパー関数を作成できる
 */

template <typename T> class Value {
public:
  explicit Value(T value) : value_(value) {}

  Value(const Value &other) : value_(other.value_) {}

  Value &operator=(const Value &other) {
    if (this == &other) {
      return *this;
    }
    value_ = other.value_;
  }

  ~Value() {}

  template <typename U, typename F> operator Result<U, F>() const {
    return Result<U, F>(value_, F(), /* has_value= */ true);
  }

private:
  T value_;
};

template <typename E> class Error {
public:
  explicit Error(E error) : error_(error) {}

  Error(const Error &other) : error_(other.error_) {}

  Error &operator=(const Error &other) {
    if (this == &other) {
      return *this;
    }
    error_ = other.error_;
  }

  ~Error() {}

  template <typename U, typename F> operator Result<U, F>() const {
    return Result<U, F>(U(), error_, /* has_value= */ false);
  }

  E error() const { return error_; }

private:
  E error_;
};

} // namespace details

/*
 * Resultクラスを使いやすくするためのヘルパー関数
 * OkとErr関数はValueとErrorクラスを生成するための関数
 * これによってテンプレートパラメータを指定しなくても暗黙の型変換でResultクラスを生成できる
 */

template <typename T> details::Value<T> Ok(T value) {
  return details::Value<T>(value);
}

template <typename E> details::Error<E> Err(E error) {
  return details::Error<E>(error);
}

#endif // WEBSERV_SRC_UTILS_RESULT_HPP