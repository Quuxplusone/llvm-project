//===---------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===---------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: libcpp-has-no-incomplete-ranges

// <span>

// template <class It, class End>
// constexpr explicit(Extent != dynamic_extent) span(It first, End last);
// Requires: [first, last) shall be a valid range.
//   If extent is not equal to dynamic_extent, then last - first shall be equal to extent.
//

#include <span>
#include <cassert>
#include <iterator>

template <class T>
constexpr bool test_ctor() {
  T val[2] = {};
  auto s1 = std::span<T>(std::begin(val), std::end(val));
  auto s2 = std::span<T, 2>(std::begin(val), std::end(val));
  assert(s1.data() == std::data(val) && s1.size() == std::size(val));
  assert(s2.data() == std::data(val) && s2.size() == std::size(val));
  return true;
}

template <size_t Extent>
constexpr void test_constructability() {
  static_assert(std::is_constructible_v<std::span<int, Extent>, int*, int*>);
  static_assert(!std::is_constructible_v<std::span<int, Extent>, const int*, const int*>);
  static_assert(!std::is_constructible_v<std::span<int, Extent>, volatile int*, volatile int*>);
  static_assert(std::is_constructible_v<std::span<const int, Extent>, int*, int*>);
  static_assert(std::is_constructible_v<std::span<const int, Extent>, const int*, const int*>);
  static_assert(!std::is_constructible_v<std::span<const int, Extent>, volatile int*, volatile int*>);
  static_assert(std::is_constructible_v<std::span<volatile int, Extent>, int*, int*>);
  static_assert(!std::is_constructible_v<std::span<volatile int, Extent>, const int*, const int*>);
  static_assert(std::is_constructible_v<std::span<volatile int, Extent>, volatile int*, volatile int*>);
  static_assert(!std::is_constructible_v<std::span<int, Extent>, int*, float*>); // types wrong
}

constexpr bool test() {
  test_constructability<std::dynamic_extent>();
  test_constructability<3>();
  struct A {};
  assert(test_ctor<int>());
  assert(test_ctor<A>());
  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;
}
