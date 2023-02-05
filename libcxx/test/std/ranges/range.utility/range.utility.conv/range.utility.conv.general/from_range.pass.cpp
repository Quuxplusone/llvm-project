//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <ranges>

// struct from_range_t { explicit from_range_t() = default; };
// inline constexpr from_range_t from_range{};

#include <cassert>
#include <type_traits>
#include <ranges>

#include "test_macros.h"

constexpr bool usable_in_constexpr(std::from_range_t) { return true; }

template <class T>
concept convertible_from_empty_braces = requires(void (*f)(T)) {
  f({});
};

int main(int, char**) {
  ASSERT_SAME_TYPE(decltype(std::from_range), const std::from_range_t);
  static_assert(usable_in_constexpr(std::from_range));
  static_assert(usable_in_constexpr(std::from_range_t()));
  static_assert(!convertible_from_empty_braces<std::from_range_t>);

  std::from_range_t fr;
  auto fr2 = fr;
  (void)fr2;

  return 0;
}
