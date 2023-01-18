//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// pair<iterator, bool> insert(const value_type& v);

#include <deque>
#include <flat_set>
#include <functional>
#include <memory_resource>
#include <string>
#include <utility>

#include "test_macros.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    using M = std::flat_set<int>;
    using R = std::pair<M::iterator, bool>;
    M m = {1,3};
    const int one = 1;
    ASSERT_SAME_TYPE(decltype(m.insert(one)), R);
    R r = m.insert(one);
    assert(r.second == false);
    assert(r.first == m.begin());
    assert(*r.first == 1);
    int two = 2;
    r = m.insert(two);
    assert(r.second == true);
    assert(r.first == m.begin() + 1);
    assert(*r.first == 2);
    r = m.insert(3);
    assert(r.second == false);
    assert(r.first == m.begin() + 2);
    assert(*r.first == 3);
    r = m.insert(4);
    assert(r.second == true);
    assert(r.first == m.begin() + 3);
    assert(*r.first == 4);
  }
  {
    using M = std::flat_set<int, std::greater<>, std::deque<int, min_allocator<int>>>;
    using R = std::pair<M::iterator, bool>;
    M m;
    m.insert(1);
    m.insert(3);
    const int one = 1;
    ASSERT_SAME_TYPE(decltype(m.insert(one)), R);
    R r = m.insert(one);
    assert(r.second == false);
    assert(r.first == m.begin() + 1);
    assert(*r.first == 1);
    int two = 2;
    r = m.insert(two);
    assert(r.second == true);
    assert(r.first == m.begin() + 1);
    assert(*r.first == 2);
    r = m.insert(3);
    assert(r.second == false);
    assert(r.first == m.begin());
    assert(*r.first == 3);
    r = m.insert(4);
    assert(r.second == true);
    assert(r.first == m.begin());
    assert(*r.first == 4);
  }
  {
    // Insertion is allocator-aware (because the vector is allocator-aware)
    using M = std::flat_set<std::pmr::string, std::less<>, std::pmr::vector<std::pmr::string>>;
    using R = std::pair<M::iterator, bool>;
    std::pmr::monotonic_buffer_resource mr;
    M m(&mr);
    std::pmr::string hello = "hello";
    ASSERT_SAME_TYPE(decltype(m.insert(hello)), R);
    R r = m.insert(hello);
    assert(r.second == true);
    assert(r.first == m.begin());
    assert(r.first->get_allocator().resource() == &mr);
  }
  return 0;
}
