//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// flat_set(sorted_unique_t, initializer_list<value_type> il, const key_compare& comp = key_compare());
// template<class Alloc> flat_set(sorted_unique_t, initializer_list<value_type> il, const key_compare& comp, const Alloc& a);

#include <cassert>
#include <deque>
#include <flat_set>
#include <functional>
#include <memory_resource>
#include <type_traits>
#include <vector>

#include "test_macros.h"
#include "min_allocator.h"
#include "../../../test_compare.h"

int main(int, char**)
{
  int expected[] = {1,2,3,5};
  {
    using C = test_less<int>;
    using M = std::flat_set<int, C>;
    auto m = M(std::sorted_unique, {1,2,3,5}, C(10));
    assert(m.size() == 4);
    assert(std::equal(m.begin(), m.end(), expected, expected+4));
    assert(m.key_comp() == C(10));
  }
  {
    // Sorting uses the comparator that was passed in
    using M = std::flat_set<int, std::function<bool(int, int)>, std::deque<int, min_allocator<int>>>;
    auto m = M(std::sorted_unique, {5,3,2,1}, std::greater<int>());
    assert(m.size() == 4);
    assert(std::equal(m.rbegin(), m.rend(), expected, expected+4));
    assert(m.key_comp()(2, 1) == true);
  }
  {
    using C = test_less<int>;
    using M = std::flat_set<int, C>;
    std::initializer_list<int> il = {1,2,3,5};
    auto m = M(std::sorted_unique, il, C(10));
    assert(m.size() == 4);
    assert(std::equal(m.begin(), m.end(), expected, expected+4));
    assert(m.key_comp() == C(10));
  }
  {
    using M = std::flat_set<short>;
    M m = M(std::sorted_unique, {1,2,3,5}, std::less<short>()); // ints implicitly converted to shorts
    assert(m.size() == 4);
    assert(std::equal(m.begin(), m.end(), expected, expected+4));
    static_assert( std::is_constructible_v<M, std::sorted_unique_t, std::initializer_list<short>, std::less<short>>);
    static_assert( std::is_constructible_v<M, std::sorted_unique_t, std::initializer_list<short>, std::less<short>, std::allocator<int>>);
    static_assert(!std::is_constructible_v<M, std::sorted_unique_t, std::initializer_list<short>, std::less<int>, std::allocator<int>>);
    static_assert(!std::is_constructible_v<M, std::sorted_unique_t, std::initializer_list<int>, std::less<short>>);
    static_assert(!std::is_constructible_v<M, std::sorted_unique_t, std::initializer_list<int>, std::less<short>, std::allocator<int>>);
    static_assert(!std::is_constructible_v<M, std::sorted_unique_t, std::initializer_list<int>, std::less<int>, std::allocator<int>>);
  }
  {
    using A = explicit_allocator<int>;
    using M = std::flat_set<int, std::greater<int>, std::deque<int, A>>;
    A a;
    M m(std::sorted_unique, {5,3,2,1}, {}, a);
    assert(std::equal(m.rbegin(), m.rend(), expected, expected+4));
  }
#if 0 // LWG 3884, flat_foo is missing allocator-extended copy/move constructors
  {
    using M = std::flat_set<int, std::less<int>, std::pmr::vector<int>>;
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::vector<M> vm(&mr);
    std::initializer_list<int> il = {1,3,4,5};
    vm.emplace_back(std::sorted_unique, il, std::less<int>());
    assert((vm[0] == M{1,3,4,5}));
    assert(std::move(vm[0]).extract().get_allocator().resource() == &mr);
  }
#endif
  return 0;
}
