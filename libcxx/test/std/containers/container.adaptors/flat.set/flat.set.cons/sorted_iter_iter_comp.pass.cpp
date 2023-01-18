//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// template <class InputIterator>
//   flat_set(sorted_unique_t, InputIterator first, InputIterator last, const key_compare& comp = key_compare());
// template<class InputIterator, class Allocator>
//   flat_set(sorted_unique_t, InputIterator first, InputIterator last, const key_compare& comp, const Allocator& a);

#include <deque>
#include <flat_set>
#include <functional>
#include <memory_resource>
#include <vector>

#include "min_allocator.h"
#include "test_allocator.h"
#include "test_iterators.h"
#include "test_macros.h"
#include "../../../test_compare.h"

int main(int, char**)
{
  {
    using M = std::flat_set<int, std::function<bool(int,int)>>;
    int ar[] = {1,2,4,5};
    auto m = M(std::sorted_unique, cpp17_input_iterator<const int*>(ar), cpp17_input_iterator<const int*>(ar + 4), std::less<int>());
    assert(m == M({1,2,4,5}, std::less<>()));
    assert(m.key_comp()(1, 2) == true);
  }
  {
    using M = std::flat_set<int, std::greater<int>, std::deque<int, min_allocator<int>>>;
    int ar[] = {5,4,2,1};
    auto m = M(std::sorted_unique, cpp17_input_iterator<const int*>(ar), cpp17_input_iterator<const int*>(ar + 4), std::greater<int>());
    assert((m == M{5,4,2,1}));
  }
  {
    // Test when the operands are of array type (also contiguous iterator type)
    using C = test_less<int>;
    using M = std::flat_set<int, C, std::vector<int, min_allocator<int>>>;
    int ar[] = {42};
    auto m = M(std::sorted_unique, ar, ar, C(5));
    assert(m.empty());
    assert(m.key_comp() == C(5));
  }
  {
    using C = test_less<int>;
    using A = test_allocator<int>;
    using M = std::flat_set<int, C, std::vector<int, A>>;
    int ar[] = {1,2,4,5};
    auto m = M(std::sorted_unique, ar, ar + 4, C(3), A(5));
    assert((m == M{1,2,4,5}));
    assert(m.key_comp() == C(3));
    assert(std::move(m).extract().get_allocator() == A(5));
  }
  {
    using A = test_allocator<int>;
    using M = std::flat_set<int, std::less<int>, std::deque<int, A>>;
    int ar[] = {1,2,4,5};
    M m = { std::sorted_unique, ar, ar + 4, {}, A(5) }; // implicit ctor
    assert((m == M{1,2,4,5}));
    assert(std::move(m).extract().get_allocator() == A(5));
  }
#if 0 // LWG ????, flat_foo is missing allocator-extended copy/move constructors
  {
    using C = test_less<int>;
    using M = std::flat_set<int, C, std::pmr::vector<int>>;
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::vector<M> vm(&mr);
    int ar[] = {1,2,4,5};
    vm.emplace_back(std::sorted_unique, cpp17_input_iterator<const int*>(ar), cpp17_input_iterator<const int*>(ar + 4), C(3));
    assert((vm[0] == M{1,2,4,5}));
    assert(vm[0].key_comp() == C(3));
    assert(std::move(vm[0]).extract().get_allocator().resource() == &mr);
  }
  {
    using C = test_less<int>;
    using M = std::flat_set<int, C, std::pmr::vector<int>>;
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::vector<M> vm(&mr);
    int ar[] = {42};
    vm.emplace_back(std::sorted_unique, ar, ar, C(4));
    assert(vm[0] == M{});
    assert(vm[0].key_comp() == C(4));
    assert(std::move(vm[0]).extract().get_allocator().resource() == &mr);
  }
#endif
  return 0;
}
