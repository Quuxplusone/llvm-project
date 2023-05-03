//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_map>

// template<container-compatible-range<value_type> R>
//   void insert_range(R&& rg);

#include <algorithm>
#include <deque>
#include <flat_map>
#include <functional>
#include <ranges>
#include <vector>

#include "test_macros.h"
#include "test_iterators.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    using P = std::pair<int, int>;
    using M = std::flat_multimap<int, int>;
    using It = forward_iterator<const P*>;
    M m = {{10,1}, {8,2}, {5,3}, {2,4}, {1,5}};
    P ar[] = {{3,1}, {1,2}, {4,3}, {1,4}, {5,5}, {9,6}};
    std::ranges::subrange r = { It(ar), It(ar + 6) };
    static_assert(std::ranges::common_range<decltype(r)>);
    m.insert_range(r);
    const P expected[] = {{1,5}, {1,2}, {1,4}, {2,4}, {3,1}, {4,3}, {5,3}, {5,5}, {8,2}, {9,6}, {10,1}};
    assert(std::ranges::is_permutation(m, expected));
    LIBCPP_ASSERT(std::ranges::equal(m, expected));
  }
  {
    using P = std::pair<int, int>;
    using M = std::flat_multimap<int, int, std::greater<>, std::deque<int, min_allocator<int>>>;
    using It = cpp20_input_iterator<const P*>;
    M m = {{8,1}, {5,2}, {3,3}, {2,4}};
    P ar[] = {{3,1}, {1,2}, {4,3}, {1,4}, {5,5}, {9,6}};
    std::ranges::subrange r = { It(ar), sentinel_wrapper<It>(It(ar + 6)) };
    static_assert(!std::ranges::common_range<decltype(r)>);
    m.insert_range(r);
    const P expected[] = {{9,6}, {8,1}, {5,2}, {5,5}, {4,3}, {3,3}, {3,1}, {2,4}, {1,2}, {1,4}};
    assert(std::ranges::is_permutation(m, expected));
    LIBCPP_ASSERT(std::ranges::equal(m, expected));
  }
  {
    // The "sorting" part uses the comparator, not operator==.
    struct ModTen {
      bool operator()(int a, int b) const { return (a % 10) < (b % 10); }
    };
    using P = std::pair<int, int>;
    using M = std::flat_multimap<int, int, ModTen>;
    M m = {{21,0}, {43,0}, {15,0}, {37,0}};
    P ar[] = {{33,1}, {18,1}, {55,1}, {18,1}, {42,1}};
    m.insert_range(ar);
    const P expected[] = {{21,0}, {42,1}, {43,0}, {33,1}, {15,0}, {55,1}, {37,0}, {18,1}, {18,1}};
    assert(std::ranges::is_permutation(m, expected));
    LIBCPP_ASSERT(std::ranges::equal(m, expected));
  }
  {
    // The "sorting" part uses the comparator, not operator==.
    struct ModTen {
      bool operator()(int a, int b) const { return (a % 10) < (b % 10); }
    };
    using P = std::pair<int, int>;
    using M = std::flat_multimap<int, int, ModTen>;
    M m = {{21,0}, {43,0}, {15,0}, {37,0}};
    P ar[] = {{33,1}, {18,1}, {55,1}, {28,1}, {42,1}};
    m.insert_range(ar);
    const P expected[] = {{21,0}, {42,1}, {43,0}, {33,1}, {15,0}, {55,1}, {37,0}, {18,1}, {28,1}};
    assert(std::ranges::is_permutation(m, expected));
    LIBCPP_ASSERT(std::ranges::equal(m, expected));
  }
  return 0;
}
