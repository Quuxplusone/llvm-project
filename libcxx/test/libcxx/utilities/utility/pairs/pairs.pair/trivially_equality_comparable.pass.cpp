//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// ADDITIONAL_COMPILE_FLAGS: -Wno-private-header

// <utility>

// template <class T1, class T2> struct pair

// pair is trivially equality comparable whenever its component types are
// trivially equality comparable, as long as there's no padding involved
// and the component types aren't references.

#include <algorithm>
#include <cassert>
#include <type_traits>
#include <utility>

#include <__type_traits/is_equality_comparable.h>

#include "test_macros.h"

struct Trivial {
  int i;
  constexpr Trivial(int i): i(i) {}
  bool operator==(const Trivial&) const = default;
};
struct NonTrivial {
  int i;
  constexpr NonTrivial(int i): i(i) {}
  constexpr bool operator==(const NonTrivial& rhs) const {
    return i == rhs.i || i == 42;
  }
};

template<class P>
constexpr bool test_impl() {
  int x = 1;
  int y = 2;
  P a[] = {{x,x}, {x,y}, {y,y}};
  P b[] = {{x,x}, {x,y}, {y,y}};
  assert(std::ranges::equal(a, b));
  assert(std::equal(a, a+3, b, b+3));
  assert(std::equal(a, a+2, b, b+2));
  assert(std::equal(a+1, a+3, b+1, b+3));
  assert(!std::equal(a, a+2, b+1, b+3));
  return true;
}

template<class T, class U, bool Expected>
void test() {
  using P = std::pair<T, U>;
  static_assert(std::__libcpp_is_trivially_equality_comparable<P, P>::value == Expected);
  static_assert(std::__libcpp_is_trivially_equality_comparable<P, const P>::value == Expected);
  static_assert(std::__libcpp_is_trivially_equality_comparable<const P, P>::value == Expected);
  static_assert(std::__libcpp_is_trivially_equality_comparable<const P, const P>::value == Expected);
  test_impl<P>();
  static_assert(test_impl<P>());
}

int main(int, char**) {
  {
    // Smoke-test: pair should be trivially equality comparable
    using P = std::pair<int, int>;
    static_assert(std::__libcpp_is_trivially_equality_comparable<P, P>::value);
  }

  test<int, int, true>();
  test<int, short, false>(); // tail padding
  test<short, int, false>(); // internal padding
  test<short, short, true>();

  test<int, int&, false>(); // non-defaulted operator==
  test<int&, int, false>(); // non-defaulted operator==
  test<int&, int&, false>(); // non-defaulted operator==

  test<Trivial, Trivial, true>();
  test<Trivial, NonTrivial, false>(); // non-trivial member
  test<NonTrivial, Trivial, false>(); // non-trivial member
  test<NonTrivial, NonTrivial, false>(); // non-trivial member

  test<short, Trivial, false>(); // internal padding

  return 0;
}
