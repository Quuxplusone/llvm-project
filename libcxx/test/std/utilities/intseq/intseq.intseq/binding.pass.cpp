//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <utility>

// template<class T, T... I>
// struct integer_sequence;

#include <utility>
#include <type_traits>
#include <cstddef>
#include <cassert>

#include "test_macros.h"

int main(int, char**)
{
  {
    auto [a, b, c] = std::integer_sequence<int, 3, 1, 4>();
    static_assert(std::is_same_v<decltype(a), int>);
    static_assert(std::is_same_v<decltype(b), int>);
    static_assert(std::is_same_v<decltype(c), int>);
    assert(a == 3);
    assert(b == 1);
    assert(c == 4);
  }
  {
    auto [a] = std::index_sequence<3>();
    static_assert(std::is_same_v<decltype(a), size_t>);
    assert(a == 3);
  }
  {
    auto [a, b] = std::make_index_sequence<2>();
    static_assert(std::is_same_v<decltype(a), size_t>);
    static_assert(std::is_same_v<decltype(b), size_t>);
    assert(a == 0);
    assert(b == 1);
  }

  return 0;
}
