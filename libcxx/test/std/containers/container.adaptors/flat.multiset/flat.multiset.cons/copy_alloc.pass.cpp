//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// flat_multiset(const flat_multiset&, const allocator_type&);

#include <cassert>
#include <flat_set>
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
    auto mo = std::flat_multiset<int, C, std::vector<int, A>>({1, 2, 3}, C(5), A(7));
    auto m = std::flat_multiset<int, C, std::vector<int, A>>(mo, A(3));

    assert(m.key_comp() == C(5));
    assert(m.size() == 3);
    std::vector<int, A> v = std::move(m).extract();
    assert(v.get_allocator() == A(3));
    assert(std::equal(v.begin(), v.end(), expected, expected + 3));

    // The original flat_multiset is unchanged.
    assert(mo.key_comp() == C(5));
    assert(mo.size() == 3);
    std::vector<int, A> vo = std::move(mo).extract();
    assert(vo.get_allocator() == A(7));
    assert(std::equal(vo.begin(), vo.end(), expected, expected + 3));
  }
  {
    int expected[] = {1,1,1,2,2,2,3,3,3};
    using C = test_less<int>;
    using FS = std::flat_multiset<int, C, std::pmr::vector<int>>;
    std::pmr::monotonic_buffer_resource mr1;
    std::pmr::monotonic_buffer_resource mr2;
    FS mo = FS({1,1,1,2,2,2,3,3,3}, C(5), &mr1);
    FS m = {mo, &mr2};  // also test the implicitness of this constructor

    assert(m.key_comp() == C(5));
    assert(m.size() == 9);
    std::pmr::vector<int> v = std::move(m).extract();
    assert(v.get_allocator().resource() == &mr2);
    assert(std::equal(v.begin(), v.end(), expected, expected + 9));

    // The original flat_multiset is unchanged.
    assert(mo.key_comp() == C(5));
    assert(mo.size() == 9);
    std::pmr::vector<int> vo = std::move(mo).extract();
    assert(vo.get_allocator().resource() == &mr1);
    assert(std::equal(vo.begin(), vo.end(), expected, expected + 9));
  }
  {
    using FS = std::flat_multiset<int, std::less<>, std::pmr::vector<int>>;
    std::pmr::vector<FS> vs;
    FS fs = {1,2,3};
    vs.push_back(fs);
    assert((vs[0] == FS{1,2,3}));
  }
  return 0;
}
