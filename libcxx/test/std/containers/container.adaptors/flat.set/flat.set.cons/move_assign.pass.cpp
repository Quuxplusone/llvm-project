//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// flat_set& operator=(flat_set&&);

#include <algorithm>
#include <deque>
#include <flat_set>
#include <functional>
#include <memory_resource>
#include <string>
#include <utility>
#include <vector>

#include "test_macros.h"
#include "MoveOnly.h"
#include "../../../test_compare.h"
#include "test_allocator.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    using C = test_less<int>;
    using A = test_allocator<int>;
    using M = std::flat_set<int, C, std::vector<int, A>>;
    M mo = M({1,2,3}, C(5), A(7));
    M m = M({}, C(3), A(7));
    m = std::move(mo);
    assert((m == M{1,2,3}));
    assert(m.key_comp() == C(5));
    assert(std::move(m).extract().get_allocator() == A(7));
    assert(mo.empty());
  }
  {
    using C = test_less<int>;
    using A = other_allocator<int>;
    using M = std::flat_set<int, C, std::deque<int, A>>;
    M mo = M({4,5}, C(5), A(7));
    M m = M({1,2,3,4}, C(3), A(7));
    m = std::move(mo);
    assert((m == M{4,5}));
    assert(m.key_comp() == C(5));
    assert(std::move(m).extract().get_allocator() == A(7));
    assert(mo.empty());
  }
  {
    using A = min_allocator<int>;
    using M = std::flat_set<int, std::greater<int>, std::vector<int, A>>;
    M mo = M({5,4,3}, A());
    M m = M({4,3,2,1}, A());
    m = std::move(mo);
    assert((m == M{5,4,3}));
    assert(std::move(m).extract().get_allocator() == A());
    assert(mo.empty());
  }
  {
    // A moved-from flat_set maintains its class invariant in the presence of moved-from elements.
    using M = std::flat_set<std::pmr::string, std::less<>, std::pmr::vector<std::pmr::string>>;
    std::pmr::monotonic_buffer_resource mr1;
    std::pmr::monotonic_buffer_resource mr2;
    M mo = M({"short", "very long string that definitely won't fit in the SSO buffer and therefore becomes empty on move"}, &mr1);
    M m = M({"don't care"}, &mr2);
    m = std::move(mo);
    assert(m.size() == 2);
    assert(std::is_sorted(m.begin(), m.end(), m.key_comp()));
    assert(m.begin()->get_allocator().resource() == &mr2);

    assert(std::is_sorted(mo.begin(), mo.end(), mo.key_comp()));
    mo.insert("foo");
    assert(mo.begin()->get_allocator().resource() == &mr1);
  }
  {
    // A moved-from flat_set maintains its class invariant in the presence of moved-from comparators.
    using C = std::function<bool(int,int)>;
    using M = std::flat_set<int, C>;
    M mo = M({1,2,3}, std::less<int>());
    M m = M({1,2}, std::greater<int>());
    m = std::move(mo);
    assert(m.size() == 3);
    assert(std::is_sorted(m.begin(), m.end(), m.key_comp()));
    assert(m.key_comp()(1,2) == true);

    assert(std::is_sorted(mo.begin(), mo.end(), mo.key_comp()));
    LIBCPP_ASSERT(m.key_comp()(1,2) == true);
    LIBCPP_ASSERT(mo.empty());
    mo.insert({1,2,3}); // insert has no preconditions
    LIBCPP_ASSERT(m == mo);
  }
  return 0;
}
