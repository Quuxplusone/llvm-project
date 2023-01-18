//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// explicit flat_set(const key_compare& comp);

#include <cassert>
#include <flat_set>
#include <functional>
#include <type_traits>

#include "test_macros.h"
#include "../../../test_compare.h"
#include "test_allocator.h"

int main(int, char**)
{
  {
    using C = test_less<int>;
    auto m = std::flat_set<int, C>(C(3));
    assert(m.empty());
    assert(m.begin() == m.end());
    assert(m.key_comp() == C(3));
    assert(m.value_comp() == C(3));
  }
  {
    // The one-argument ctor is explicit.
    using C = test_less<int>;
    static_assert(std::is_constructible_v<std::flat_set<int, C>, C>);
    static_assert(!std::is_convertible_v<C, std::flat_set<int, C>>);

    static_assert(std::is_constructible_v<std::flat_set<int>, std::less<int>>);
    static_assert(!std::is_convertible_v<std::less<int>, std::flat_set<int>>);
  }
  return 0;
}
