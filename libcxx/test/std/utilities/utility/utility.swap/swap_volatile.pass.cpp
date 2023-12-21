//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <utility>

#include <algorithm>
#include <cassert>
#include <utility>

#include "test_macros.h"

struct S {
  S(int i) : i_(i) {}
  S(S&& s) : i_(s.i_) {}
  S(const volatile S& s) : i_(s.i_) {}
  S(volatile S&& s) : i_(s.i_) {}
  S& operator=(const volatile S& s) { i_ = s.i_; return *this; }
  S& operator=(volatile S&& s) { i_ = s.i_; return *this; }
  volatile S& operator=(const volatile S& s) volatile { i_ = s.i_; return *this; }
  volatile S& operator=(volatile S&& s) volatile { i_ = s.i_; return *this; }
  bool operator==(int i) const volatile { return i_ == i; }
  int i_;
};
static_assert(std::is_move_constructible<volatile S>::value, "");
static_assert(std::is_move_assignable<volatile S>::value, "");

// This function is not TEST_CONSTEXPR_CXX17 because it's
// ill-formed to perform a volatile read or write (even of
// an int) during constexpr evaluation.
//
template <class T>
bool test_swap() {
  volatile T a[2] = {1, 2};
  volatile T b[2] = {3, 4};

  std::swap(a[0], a[1]);
  assert(a[0] == 2 && a[1] == 1);

  std::swap(a, b);
  assert(a[0] == 3 && a[1] == 4);
  assert(b[0] == 2 && b[1] == 1);

  std::swap_ranges(a, a+2, b);
  assert(a[0] == 2 && a[1] == 1);
  assert(b[0] == 3 && b[1] == 4);

#if TEST_STD_VER >= 20
  std::ranges::swap(a[0], a[1]);
  assert(a[0] == 1 && a[1] == 2);

  std::ranges::swap(a, b);
  assert(a[0] == 3 && a[1] == 4);
  assert(b[0] == 1 && b[1] == 2);

  std::ranges::swap_ranges(a, b);
  assert(a[0] == 1 && a[1] == 2);
  assert(b[0] == 3 && b[1] == 4);

  std::ranges::swap_ranges(a, a+2, b, b+2);
  assert(a[0] == 3 && a[1] == 4);
  assert(b[0] == 1 && b[1] == 2);
#endif // TEST_STD_VER >= 20

  return true;
}

int main(int, char**)
{
  test_swap<int>();
  test_swap<S>();

  return 0;
}
