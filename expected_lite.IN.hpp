// Copyright (C) 2013 Martin Moene.
//
// Implementation note: this version targets C++03.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// The idea is based on the Expected<T> class template authored by
// Andrei Alexandrescu. The interface is based on the std::optional proposal
// authored by Fernando Cacciola and Andrzej Krzemienski and the
// Boost.Expected proposal authored by Pierre Talbot.

#ifndef NONSTD_EXPECTED_HPP
#define NONSTD_EXPECTED_HPP

#include <exception>

#if __cplusplus < 201103L       // non-C++11 compilers
# include "exception_c++11.h"   // std::exception_ptr, std::move()
#endif

namespace nonstd {
  // 20.5.4, expected for object types
  template <class T> class expected;

  // 20.5.5, In-place construction
  struct in_place_t{};
  constexpr in_place_t in_place{};

  // 20.5.6, Disengaged state indicator
  struct nullexp_t{see below};
  constexpr nullexp_t nullexp(unspecified);

  // 20.5.7, class bad_expected_access
  class bad_expected_access;

  template <class T>
  class expected
  {
  public:
    typedef T value_type;

    // 20.5.4.1, constructors
    constexpr expected() noexcept;
    constexpr expected(nullexp_t) noexcept;
    expected(const expected&);
    expected(expected&&) noexcept(see below);
    constexpr expected(const T&);
    constexpr expected(T&&);
    template <class... Args> constexpr explicit expected(in_place_t, Args&&...);
    template <class U, class... Args>
      constexpr explicit expected(in_place_t, initializer_list<U>, Args&&...);

    // 20.5.4.2, destructor
    ~expected();

    // 20.5.4.3, assignment
    expected& operator=(nullexp_t) noexcept;
    expected& operator=(const expected&);
    expected& operator=(expected&&) noexcept(see below);
    template <class U> expected& operator=(U&&);
    template <class... Args> void emplace(Args&&...);
    template <class U, class... Args>
      void emplace(initializer_list<U>, Args&&...);

    // 20.5.4.4, swap
    void swap(expected&) noexcept(see below);

    // 20.5.4.5, observers
    constexpr T const* operator ->() const;
    T* operator ->();
    constexpr T const& operator *() const;
    T& operator *();
    constexpr explicit operator bool() const noexcept;
    constexpr T const& value() const;
    T& value();
    template <class U> constexpr T value_or(U&&) const&;
    template <class U> T value_or(U&&) &&;

  private:
    bool init; // exposition only
    T*   val;  // exposition only
  };

  // 20.5.8, Relational operators
  template <class T> constexpr bool operator==(const expected<T>&, const expected<T>&);
  template <class T> constexpr bool operator<(const expected<T>&, const expected<T>&);

  // 20.5.9, Comparison with nullexp
  template <class T> constexpr bool operator==(const expected<T>&, nullexp_t) noexcept;
  template <class T> constexpr bool operator==(nullexp_t, const expected<T>&) noexcept;
  template <class T> constexpr bool operator<(const expected<T>&, nullexp_t) noexcept;
  template <class T> constexpr bool operator<(nullexp_t, const expected<T>&) noexcept;

  // 20.5.10, Comparison with T
  template <class T> constexpr bool operator==(const expected<T>&, const T&);
  template <class T> constexpr bool operator==(const T&, const expected<T>&);
  template <class T> constexpr bool operator<(const expected<T>&, const T&);
  template <class T> constexpr bool operator<(const T&, const expected<T>&);

  // 20.5.11, Specialized algorithms
  template <class T> void swap(expected<T>&, expected<T>&) noexcept(see below);
  template <class T> constexpr expected<see below> make_expected(T&&);

  // 20.5.12, hash support
  template <class T> struct hash;
  template <class T> struct hash<expected<T>>;
} // namespace nonstd

#endif // NONSTD_EXPECTED_HPP
