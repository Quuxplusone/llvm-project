//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// flat_set& operator=(const flat_set& s);

#include <deque>
#include <flat_set>
#include <functional>
#include <memory_resource>
#include <vector>

#include "test_macros.h"
#include "../../../test_compare.h"
#include "test_allocator.h"

int main(int, char**)
{
  {
    // test_allocator is not propagated
    using C = test_less<int>;
    using A = test_allocator<int>;
    using FS = std::flat_set<int, C, std::vector<int, A>>;
    auto mo = FS({1,2,4,7}, C(5), A(2));
    auto m = FS({3,4,5}, C(3), A(7));
    m = mo;
    assert((m == FS{1,2,4,7}));
    assert(m.key_comp() == C(5));
    assert(std::move(m).extract().get_allocator() == A(7));

    // mo is unchanged
    assert((mo == FS{1,2,4,7}));
    assert(mo.key_comp() == C(5));
    assert(std::move(mo).extract().get_allocator() == A(2));
  }
  {
    // other_allocator is propagated
    using C = test_less<int>;
    using A = other_allocator<int>;
    using FS = std::flat_set<int, C, std::vector<int, A>>;
    auto mo = FS({1,2,4,7}, C(5), A(2));
    auto m = FS({3,4,5}, C(3), A(7));
    m = mo;
    assert((m == FS{1,2,4,7}));
    assert(m.key_comp() == C(5));
    assert(std::move(m).extract().get_allocator() == A(2));

    // mo is unchanged
    assert((mo == FS{1,2,4,7}));
    assert(mo.key_comp() == C(5));
    assert(std::move(mo).extract().get_allocator() == A(2));
  }
  {
    // pmr allocator is not propagated
    using C = std::flat_set<int, std::less<>, std::pmr::deque<int>>;
    std::pmr::monotonic_buffer_resource mr1;
    std::pmr::monotonic_buffer_resource mr2;
    C mo = C({1,2,3}, &mr1);
    C m = C({4,5}, &mr2);
    m = mo;
    assert((m == C{1,2,3}));
    assert(std::move(m).extract().get_allocator().resource() == &mr2);

    // mo is unchanged
    assert((mo == C{1,2,3}));
    assert(std::move(mo).extract().get_allocator().resource() == &mr1);
  }
  {
    // comparator is copied and invariant is preserved
    using C = std::flat_set<int, std::function<bool(int, int)>>;
    C mo = C({1,2,3,4}, std::less<int>());
    C m = C({1,2,3,4}, std::greater<int>());
    assert(m.key_comp()(2, 1) == true);
    assert(m != mo);
    m = mo;
    assert(m.key_comp()(2, 1) == false);
    assert(m == mo);
  }
  {
    // self-assignment
    using C = std::flat_set<int>;
    C m = {1,2,3,4};
    m = static_cast<const C&>(m);
    assert((m == C{1,2,3,4}));
  }
  return 0;
}
