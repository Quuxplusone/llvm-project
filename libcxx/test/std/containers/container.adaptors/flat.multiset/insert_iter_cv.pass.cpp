//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// iterator insert(const_iterator position, const value_type& x);

#include <algorithm>
#include <cassert>
#include <deque>
#include <flat_set>
#include <functional>
#include <memory_resource>
#include <string>
#include <vector>

#include "test_macros.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    using M = std::flat_multiset<int>;
    M m;
    int one = 1;
    ASSERT_SAME_TYPE(M::iterator, decltype(m.insert(m.end(), one)));
    ASSERT_SAME_TYPE(M::iterator, decltype(m.insert(m.cend(), one)));
    M::iterator it = m.insert(m.cend(), one);
    assert(it == m.begin());
    assert(m.size() == 1);
    int two = 2;
    it = m.insert(m.end(), two);
    assert(it == m.begin() + 1);
    assert(m.size() == 2);
    it = m.insert(m.begin() + 1, one); // duplicate
    assert(it == m.begin() + 1);
    assert(m.size() == 3);
    int three = 3;
    it = m.insert(m.cbegin(), three);
    assert(it == m.begin() + 3);
    assert(m.size() == 4);
    it = m.insert(m.cend(), three); // duplicate
    assert(it == m.begin() + 4);
    assert(m.size() == 5);
  }
  {
    using M = std::flat_multiset<int, std::greater<>, std::deque<int, min_allocator<int>>>;
    M m;
    int one = 1;
    ASSERT_SAME_TYPE(M::iterator, decltype(m.insert(m.end(), one)));
    ASSERT_SAME_TYPE(M::iterator, decltype(m.insert(m.cend(), one)));
    M::iterator it = m.insert(m.cend(), one);
    assert(it == m.begin());
    assert(m.size() == 1);
    int two = 2;
    it = m.insert(m.end(), two);
    assert(it == m.begin());
    assert(m.size() == 2);
    it = m.insert(m.begin() + 1, one); // duplicate
    assert(it == m.begin() + 1);
    assert(m.size() == 3);
    int three = 3;
    it = m.insert(m.cbegin(), three); 
    assert(it == m.begin());
    assert(m.size() == 4);
    it = m.insert(m.cend(), three); // duplicate
    assert(it == m.begin() + 1);
    assert(m.size() == 5);
  }
  {
    // Insertion is allocator-aware (because the vector is allocator-aware)
    using M = std::flat_multiset<std::pmr::string, std::less<>, std::pmr::vector<std::pmr::string>>;
    std::pmr::monotonic_buffer_resource mr;
    M m(&mr);
    std::pmr::string hello = "hello";
    ASSERT_SAME_TYPE(M::iterator, decltype(m.insert(m.begin(), hello)));
    M::iterator it = m.insert(m.begin(), hello);
    assert(it == m.begin());
    assert(it->get_allocator().resource() == &mr);
  }
  {
    // Make sure the insertion point is always closest to the hint.
    struct ByTensDigit {
      bool operator()(int a, int b) const { return a/10 < b/10; }
    };
    using M = std::flat_multiset<int, ByTensDigit>;
    M m;
    M::iterator it;
    it = m.insert(m.end(), 20); // hint is correct, at begin and end
    assert((m == M{20}));
    assert(it == m.begin());
    it = m.insert(m.end(), 21); // hint is correct, at end
    assert(std::ranges::equal(m, std::vector{20, 21}));
    assert(it == m.begin() + 1);
    it = m.insert(m.begin(), 22); // hint is correct, at begin
    assert(std::ranges::equal(m, std::vector{22, 20, 21}));
    assert(it == m.begin());

    it = m.insert(m.begin(), 10); // hint is correct, at begin
    assert(std::ranges::equal(m, std::vector{10, 22, 20, 21}));
    assert(it == m.begin());
    it = m.insert(m.begin() + 1, 11); // hint is correct, at upper bound
    assert(std::ranges::equal(m, std::vector{10, 11, 22, 20, 21}));
    assert(it == m.begin() + 1);
    it = m.insert(m.begin(), 12); // hint is correct, at lower bound
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 22, 20, 21}));
    assert(it == m.begin());

    it = m.insert(m.end(), 30); // hint is correct, at end
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 22, 20, 21, 30}));
    assert(it == m.begin() + 6);
    it = m.insert(m.end() - 1, 31); // hint is correct, at lower bound
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 22, 20, 21, 31, 30}));
    assert(it == m.begin() + 6);
    it = m.insert(m.end(), 32); // hint is correct, at upper bound
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 22, 20, 21, 31, 30, 32}));
    assert(it == m.begin() + 8);

    it = m.insert(m.begin(), 23); // hint is wrong, at begin
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 23, 22, 20, 21, 31, 30, 32}));
    assert(it == m.begin() + 3);
    it = m.insert(m.end(), 24); // hint is wrong, at end
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 23, 22, 20, 21, 24, 31, 30, 32}));
    assert(it == m.begin() + 7);
    it = m.insert(m.begin() + 2, 25); // hint is wrong, toward begin
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 25, 23, 22, 20, 21, 24, 31, 30, 32}));
    assert(it == m.begin() + 3);
    it = m.insert(m.end() - 2, 26); // hint is wrong, toward end
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 25, 23, 22, 20, 21, 24, 26, 31, 30, 32}));
    assert(it == m.begin() + 9);

    it = m.insert(m.begin() + 5, 27); // hint is correct, in middle
    assert(std::ranges::equal(m, std::vector{12, 10, 11, 25, 23, 27, 22, 20, 21, 24, 26, 31, 30, 32}));
    assert(it == m.begin() + 5);
  }
  return 0;
}
