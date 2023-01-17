//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// flat_set(const flat_set&, const allocator_type&);

#include <cassert>
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
    int expected[] = {1, 2, 3};
    using C = test_less<int>;
    using A = test_allocator<int>;
    auto mo = std::flat_set<int, C, std::vector<int, A>>({1, 2, 3}, C(5), A(7));
    auto m = std::flat_set<int, C, std::vector<int, A>>(mo, A(3));

    assert(m.key_comp() == C(5));
    assert(m.size() == 3);
    std::vector<int, A> v = std::move(m).extract();
    assert(v.get_allocator() == A(3));
    assert(std::equal(v.begin(), v.end(), expected, expected + 3));

    // mo is unchanged
    assert(mo.key_comp() == C(5));
    assert(mo.size() == 3);
    std::vector<int, A> vo = std::move(mo).extract();
    assert(vo.get_allocator() == A(7));
    assert(std::equal(vo.begin(), vo.end(), expected, expected + 3));
  }
  {
    int expected[] = {1, 2, 3};
    using C = test_less<int>;
    using M = std::flat_set<int, C, std::pmr::vector<int>>;
    std::pmr::monotonic_buffer_resource mr1;
    std::pmr::monotonic_buffer_resource mr2;
    M mo = M({1,1,1,2,2,2,3,3,3}, C(5), &mr1);
    M m = {mo, &mr2};  // also test the implicitness of this constructor

    assert(m.key_comp() == C(5));
    assert(m.size() == 3);
    std::pmr::vector<int> v = std::move(m).extract();
    assert(v.get_allocator().resource() == &mr2);
    assert(std::equal(v.begin(), v.end(), expected, expected + 3));

    // mo is unchanged
    assert(mo.key_comp() == C(5));
    assert(mo.size() == 3);
    std::pmr::vector<int> vo = std::move(mo).extract();
    assert(vo.get_allocator().resource() == &mr1);
    assert(std::equal(vo.begin(), vo.end(), expected, expected + 3));
  }
  {
    using M = std::flat_set<int, std::less<>, std::pmr::vector<int>>;
    std::pmr::vector<M> vs;
    M m = {1,2,3};
    vs.push_back(m);
    assert((vs[0] == M{1,2,3}));
  }
  return 0;
}
