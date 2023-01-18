//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// void swap(flat_set& y) noexcept;
// friend void swap(flat_set& x, flat_set& y) noexcept;

#include <cassert>
#include <deque>
#include <flat_set>
#include <type_traits>
#include <vector>

#include "test_macros.h"
#include "MoveOnly.h"
#include "test_allocator.h"

struct ThrowingSwapComp {
    explicit ThrowingSwapComp() = default;
    ThrowingSwapComp(const ThrowingSwapComp&) noexcept(false) {}
    bool operator()(const auto&, const auto&) const { return false; }
};

int main(int, char**)
{
  {
    using C = std::flat_set<MoveOnly>;
    C c;
    static_assert(noexcept(c.swap(c)));
    static_assert(noexcept(swap(c, c)));
    static_assert(std::is_nothrow_swappable_v<C>);
  }
  {
    using C = std::flat_set<MoveOnly, std::less<MoveOnly>, std::deque<MoveOnly, test_allocator<MoveOnly>>>;
    C c;
    static_assert(noexcept(c.swap(c)));
    static_assert(noexcept(swap(c, c)));
    static_assert(std::is_nothrow_swappable_v<C>);
  }
  {
    using C = std::flat_set<MoveOnly, std::greater<>, std::vector<MoveOnly, other_allocator<MoveOnly>>>;
    C c;
    static_assert(noexcept(c.swap(c)));
    static_assert(noexcept(swap(c, c)));
    static_assert(std::is_nothrow_swappable_v<C>);
  }
  {
    // swap is unconditionally noexcept
    using C = std::flat_set<MoveOnly, ThrowingSwapComp>;
    C c;
    static_assert(noexcept(c.swap(c)));
    static_assert(noexcept(swap(c, c)));
    static_assert(std::is_nothrow_swappable_v<C>);
  }
  return 0;
}
