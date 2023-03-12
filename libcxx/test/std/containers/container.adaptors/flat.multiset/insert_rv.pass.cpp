//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// iterator insert(value_type&& v);

#include <deque>
#include <flat_set>
#include <functional>
#include <memory_resource>
#include <string>
#include <vector>

#include "test_macros.h"
#include "MoveOnly.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    using M = std::flat_multiset<int, std::less<int>, std::vector<int, min_allocator<int>>>;
    using R = M::iterator;
    M m;
    R r = m.insert(2);
    assert(r == m.begin());
    assert(m.size() == 1);
    assert(*r == 2);

    r = m.insert(1);
    assert(r == m.begin());
    assert(m.size() == 2);
    assert(*r == 1);

    r = m.insert(3);
    assert(r == m.begin() + 2);
    assert(m.size() == 3);
    assert(*r == 3);

    r = m.insert(3);
    assert(r == m.begin() + 3);
    assert(m.size() == 4);
    assert(*r == 3);
  }
  {
    using M = std::flat_multiset<MoveOnly, std::greater<>, std::deque<MoveOnly, min_allocator<MoveOnly>>>;
    using R = M::iterator;
    M m;
    m.insert(1);
    m.insert(3);
    R r = m.insert(1);
    assert(r == m.begin() + 2);
    assert(*r == 1);
    r = m.insert(2);
    assert(r == m.begin() + 1);
    assert(*r == 2);
    r = m.insert(3);
    assert(r == m.begin() + 1);
    assert(*r == 3);
    r = m.insert(3);
    assert(r == m.begin() + 2);
    assert(*r == 3);
  }
  {
    // Insertion is allocator-aware (because the vector is allocator-aware)
    using M = std::flat_multiset<std::pmr::string, std::less<>, std::pmr::vector<std::pmr::string>>;
    using R = M::iterator;
    std::pmr::monotonic_buffer_resource mr;
    M m(&mr);
    ASSERT_SAME_TYPE(decltype(m.insert("hello")), R);
    R r = m.insert("hello");
    assert(r == m.begin());
    assert(r->get_allocator().resource() == &mr);
  }
  return 0;
}
