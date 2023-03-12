//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// template<class Allocator>
//   explicit flat_multiset(const Allocator& a);

#include <cassert>
#include <flat_set>
#include <functional>
#include <memory_resource>
#include <vector>

#include "test_macros.h"
#include "test_allocator.h"

int main(int, char**)
{
  {
    using A = test_allocator<int>;
    std::flat_multiset<int, std::less<int>, std::vector<int, A>> m(A(5));
    assert(m.empty());
    assert(m.begin() == m.end());
    assert(std::move(m).extract().get_allocator() == A(5));
  }
  {
    using M = std::flat_multiset<int, std::less<int>, std::pmr::vector<int>>;
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::polymorphic_allocator<int> pa = &mr;
    auto m1 = M(pa);
    assert(m1.empty());
    assert(std::move(m1).extract().get_allocator() == pa);
    auto m2 = M(&mr);
    assert(m2.empty());
    assert(std::move(m2).extract().get_allocator() == pa);
  }
  return 0;
}
