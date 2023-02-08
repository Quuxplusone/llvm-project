//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// flat_set(initializer_list<value_type> il, const key_compare& comp = key_compare());
// template<class Alloc> flat_set(initializer_list<value_type> il, const Alloc& a);

#include <cassert>
#include <deque>
#include <flat_set>
#include <functional>
#include <memory_resource>
#include <type_traits>
#include <vector>

#include "test_macros.h"
#include "min_allocator.h"

struct DefaultCtableComp {
  explicit DefaultCtableComp() { default_constructed_ = true; }
  bool operator()(int, int) const { return false; }
  bool default_constructed_ = false;
};

int main(int, char**)
{
  int expected[] = {1,2,3,5};
  {
    using M = std::flat_set<int>;
    M m = {5,2,2,3,1,3};
    assert(std::equal(m.begin(), m.end(), expected, expected+4));
  }
  {
    using M = std::flat_set<int, std::greater<int>, std::deque<int, min_allocator<int>>>;
    M m = {5,2,2,3,1,3};
    assert(std::equal(m.rbegin(), m.rend(), expected, expected+4));
  }
  {
    using M = std::flat_set<int, std::greater<int>, std::deque<int, min_allocator<int>>>;
    std::initializer_list<int> il = {5,2,2,3,1,3};
    M m = il;
    assert(std::equal(m.rbegin(), m.rend(), expected, expected+4));
  }
  {
    using M = std::flat_set<short>;
    M m = {5,2,2,3,1,3}; // ints implicitly converted to shorts
    assert(std::equal(m.begin(), m.end(), expected, expected+4));
    static_assert( std::is_constructible_v<M, std::initializer_list<short>>);
    static_assert( std::is_constructible_v<M, std::initializer_list<short>, std::allocator<int>>);
    static_assert(!std::is_constructible_v<M, std::initializer_list<int>>);
    static_assert(!std::is_constructible_v<M, std::initializer_list<int>, std::allocator<int>>);
  }
  {
    using A = explicit_allocator<int>;
    {
      using M = std::flat_set<int, DefaultCtableComp, std::vector<int, A>>;
      M m = {1,2,3};
      assert(m.size() == 1);
      LIBCPP_ASSERT(*m.begin() == 1);
      assert(m.key_comp().default_constructed_);
    }
    {
      using M = std::flat_set<int, std::greater<int>, std::deque<int, A>>;
      A a;
      M m({5,2,2,3,1,3}, a);
      assert(std::equal(m.rbegin(), m.rend(), expected, expected+4));
    }
  }
  {
    using M = std::flat_set<int, std::less<int>, std::pmr::vector<int>>;
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::vector<M> vm(&mr);
    std::initializer_list<int> il = {3,1,4,1,5};
    vm.emplace_back(il);
    assert((vm[0] == M{1,3,4,5}));
    assert(std::move(vm[0]).extract().get_allocator().resource() == &mr);
  }
  return 0;
}
