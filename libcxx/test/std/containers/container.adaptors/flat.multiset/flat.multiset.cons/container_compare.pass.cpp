//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// explicit flat_multiset(container_type cont);
// template<class Allocator>
//   flat_multiset(const container_type& cont, const Allocator& a);

#include <deque>
#include <flat_set>
#include <functional>
#include <memory_resource>
#include <vector>

#include "min_allocator.h"
#include "MoveOnly.h"
#include "test_allocator.h"
#include "test_iterators.h"
#include "test_macros.h"
#include "../../../test_compare.h"

int main(int, char**)
{
  using C = test_less<int>;
  {
    using M = std::flat_multiset<int, C>;
    std::vector<int> v = {1,1,1,2,2,3,2,3,3};
    auto m = M(v, C(10));
    assert((m == M{1,1,1,2,2,2,3,3,3}));
    assert(m.key_comp() == C(10));
    m = M(std::move(v), C(11));
    assert(v.empty()); // it was moved-from
    assert((m == M{1,1,1,2,2,2,3,3,3}));
    assert(m.key_comp() == C(11));
  }
  {
    using V = std::deque<int, min_allocator<int>>;
    using M = std::flat_multiset<int, C, V>;
    V v = {1,1,1,2,2,3,2,3,3};
    auto m = M(v, C(10));
    assert((m == M{1,1,1,2,2,2,3,3,3}));
    assert(m.key_comp() == C(10));
    m = M(std::move(v), C(12));
    assert(v.empty()); // it was moved-from
    assert((m == M{1,1,1,2,2,2,3,3,3}));
    assert(m.key_comp() == C(12));
  }
  {
    using A = test_allocator<int>;
    using M = std::flat_multiset<int, C, std::vector<int, A>>;
    auto v = std::vector<int, A>({1,1,1,2,2,3,2,3,3}, A(5));
    auto m = M(std::move(v), C(13));
    assert(v.empty()); // it was moved-from
    assert((m == M{1,1,1,2,2,2,3,3,3}));
    assert(m.key_comp() == C(13));
    assert(std::move(m).extract().get_allocator() == A(5));
  }
  {
    using A = test_allocator<int>;
    using M = std::flat_multiset<int, C, std::vector<int, A>>;
    auto v = std::vector<int, A>({1,1,1,2,2,3,2,3,3}, A(4));
    auto m = M(v, C(2), A(5)); // replaces v's allocator
    assert(!v.empty()); // it was an lvalue above
    assert((m == M{1,1,1,2,2,2,3,3,3}));
    assert(m.key_comp() == C(2));
    assert(std::move(m).extract().get_allocator() == A(5));
  }
  {
    using A = test_allocator<int>;
    using M = std::flat_multiset<int, C, std::deque<int, A>>;
    auto v = std::deque<int, A>({1,1,1,2,2,3,2,3,3}, A(4));
    M m = { v, C(2), A(5) }; // implicit ctor
    assert(!v.empty()); // it was an lvalue above
    assert((m == M{1,1,1,2,2,2,3,3,3}));
    assert(m.key_comp() == C(2));
    assert(std::move(m).extract().get_allocator() == A(5));
  }
  {
    using M = std::flat_multiset<int, C, std::pmr::vector<int>>;
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::vector<M> vm(&mr);
    std::pmr::vector<int> v = {1,1,1,2,2,3,2,3,3};
    assert(v.get_allocator().resource() != &mr);
    vm.emplace_back(v, C(2));
    assert(v.size() == 9); // v's value is unchanged, since it was an lvalue above
    assert((vm[0] == M{1,1,1,2,2,2,3,3,3}));
    assert(vm[0].key_comp() == C(2));
    assert(std::move(vm[0]).extract().get_allocator().resource() == &mr);
  }
  {
    using M = std::flat_multiset<int, C, std::pmr::vector<int>>;
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::vector<M> vm(&mr);
    std::pmr::vector<int> v = {1,1,1,2,2,3,2,3,3};
    assert(v.get_allocator().resource() != &mr);
    vm.emplace_back(std::move(v), C(2));
    LIBCPP_ASSERT(v.size() == 9); // v is untouched, since it uses a different allocator
    assert((vm[0] == M{1,1,1,2,2,2,3,3,3}));
    assert(vm[0].key_comp() == C(2));
    assert(std::move(vm[0]).extract().get_allocator().resource() == &mr);
  }
  {
    using M = std::flat_multiset<int, C, std::pmr::vector<int>>;
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::vector<M> vm(&mr);
    std::pmr::vector<int> v({1,1,1,2,2,3,2,3,3}, &mr);
    assert(v.get_allocator().resource() == &mr);
    vm.emplace_back(std::move(v), C(2));
    assert(v.empty());  // v is moved-from (after LWG 3802)
    assert((vm[0] == M{1,1,1,2,2,2,3,3,3}));
    assert(vm[0].key_comp() == C(2));
    assert(std::move(vm[0]).extract().get_allocator().resource() == &mr);
  }
  {
    using CC = test_less<MoveOnly>;
    using M = std::flat_multiset<MoveOnly, CC, std::pmr::vector<MoveOnly>>;
    std::pmr::vector<M> vm;
    std::pmr::vector<MoveOnly> v;
    vm.emplace_back(std::move(v), CC(2)); // this was a hard error before LWG 3802
    assert(vm.size() == 1);
    assert(vm[0].key_comp() == CC(2));
  }
  return 0;
}
