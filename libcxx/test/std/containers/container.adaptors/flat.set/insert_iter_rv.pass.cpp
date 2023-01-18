//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// iterator insert(const_iterator position, value_type&& x);

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
    using M = std::flat_set<int>;
    M m;
    ASSERT_SAME_TYPE(M::iterator, decltype(m.insert(m.end(), 1)));
    ASSERT_SAME_TYPE(M::iterator, decltype(m.insert(m.cend(), 1)));
    M::iterator it = m.insert(m.cend(), 1);
    assert(it == m.begin());
    assert(m.size() == 1);
    it = m.insert(m.end(), 2);
    assert(it == m.begin() + 1);
    assert(m.size() == 2);
    it = m.insert(m.begin() + 1, 1); // insertion fails
    assert(it == m.begin());
    assert(m.size() == 2);
    it = m.insert(m.cbegin(), 3);
    assert(it == m.begin() + 2);
    assert(m.size() == 3);
    it = m.insert(m.cend(), 3); // insertion fails
    assert(it == m.begin() + 2);
    assert(m.size() == 3);
  }
  {
    using M = std::flat_set<int, std::greater<>, std::deque<int, min_allocator<int>>>;
    M m;
    ASSERT_SAME_TYPE(M::iterator, decltype(m.insert(m.end(), 1)));
    ASSERT_SAME_TYPE(M::iterator, decltype(m.insert(m.cend(), 1)));
    M::iterator it = m.insert(m.cend(), 1);
    assert(it == m.begin());
    assert(m.size() == 1);
    it = m.insert(m.end(), 2);
    assert(it == m.begin());
    assert(m.size() == 2);
    it = m.insert(m.begin() + 1, 1); // insertion fails
    assert(it == m.begin() + 1);
    assert(m.size() == 2);
    it = m.insert(m.cbegin(), 3); 
    assert(it == m.begin());
    assert(m.size() == 3);
    it = m.insert(m.cend(), 3); // insertion fails
    assert(it == m.begin());
    assert(m.size() == 3);
  }
  {
    // Insertion is allocator-aware (because the vector is allocator-aware)
    using M = std::flat_set<std::pmr::string, std::less<>, std::pmr::vector<std::pmr::string>>;
    std::pmr::monotonic_buffer_resource mr;
    M m(&mr);
    ASSERT_SAME_TYPE(M::iterator, decltype(m.insert(m.begin(), "hello")));
    M::iterator it = m.insert(m.begin(), "hello");
    assert(it == m.begin());
    assert(it->get_allocator().resource() == &mr);
  }
  return 0;
}
