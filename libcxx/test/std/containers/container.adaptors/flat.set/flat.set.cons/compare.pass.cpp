//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// explicit flat_set(const key_compare& comp);
// template <class Alloc>
//   flat_set(const key_compare& comp, const Alloc& a);

#include <deque>
#include <flat_set>
#include <functional>
#include <memory_resource>
#include <type_traits>
#include <vector>

#include "test_macros.h"
#include "../../../test_compare.h"
#include "test_allocator.h"

int main(int, char**)
{
  {
    using C = test_less<int>;
    auto m = std::flat_set<int, C>(C(3));
    assert(m.empty());
    assert(m.begin() == m.end());
    assert(m.key_comp() == C(3));
    assert(m.value_comp() == C(3));
  }
  {
    // The one-argument ctor is explicit.
    using C = test_less<int>;
    static_assert(std::is_constructible_v<std::flat_set<int, C>, C>);
    static_assert(!std::is_convertible_v<C, std::flat_set<int, C>>);

    static_assert(std::is_constructible_v<std::flat_set<int>, std::less<int>>);
    static_assert(!std::is_convertible_v<std::less<int>, std::flat_set<int>>);
  }
  {
    using C = test_less<int>;
    using A = test_allocator<int>;
    auto m = std::flat_set<int, C, std::vector<int, A>>(C(4), A(5));
    assert(m.empty());
    assert(m.begin() == m.end());
    assert(m.key_comp() == C(4));
    assert(std::move(m).extract().get_allocator() == A(5));
  }
  {
    using C = test_less<int>;
    using A = test_allocator<int>;
    std::flat_set<int, C, std::deque<int, A>> m = { C(4), A(5) }; // implicit ctor
    assert(m.empty());
    assert(m.begin() == m.end());
    assert(m.key_comp() == C(4));
    assert(std::move(m).extract().get_allocator() == A(5));
  }
  {
    using M = std::flat_set<int, std::function<bool(int, int)>, std::pmr::vector<int>>;
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::vector<M> vm(&mr);
    vm.emplace_back(std::greater<int>());
    assert(vm[0] == M{});
    assert(vm[0].key_comp()(2, 1) == true);
    assert(std::move(vm[0]).extract().get_allocator().resource() == &mr);
  }
  return 0;
}
