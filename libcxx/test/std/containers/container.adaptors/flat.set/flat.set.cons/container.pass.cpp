//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// explicit flat_set(container_type cont);
// template<class Allocator>
//   flat_set(const container_type& cont, const Allocator& a);

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

int main(int, char**)
{
  {
    using M = std::flat_set<int>;
    std::vector<int> v = {1,1,1,2,2,3,2,3,3};
    auto m = M(v);
    assert((m == M{1,2,3}));
    m = M(std::move(v));
    assert(v.empty()); // it was moved-from
    assert((m == M{1,2,3}));
    static_assert(std::is_constructible_v<M, std::vector<int>>);
    static_assert(!std::is_assignable_v<M&, std::vector<int>>);
    static_assert(!std::is_convertible_v<std::vector<int>, M>);
  }
  {
    using V = std::deque<int, min_allocator<int>>;
    using M = std::flat_set<int, std::greater<int>, V>;
    V v = {1,1,1,2,2,3,2,3,3};
    auto m = M(v);
    assert((m == M{3,2,1}));
    m = M(std::move(v));
    assert(v.empty()); // it was moved-from
    assert((m == M{3,2,1}));
    static_assert(std::is_constructible_v<M, V>);
    static_assert(!std::is_assignable_v<M&, V>);
    static_assert(!std::is_convertible_v<V, M>);
  }
  {
    using A = test_allocator<int>;
    using M = std::flat_set<int, std::less<int>, std::vector<int, A>>;
    auto v = std::vector<int, A>({1,1,1,2,2,3,2,3,3}, A(5));
    auto m = M(std::move(v));
    assert(v.empty()); // it was moved-from
    assert((m == M{1,2,3}));
    assert(std::move(m).extract().get_allocator() == A(5));
  }
  {
    using A = test_allocator<int>;
    using M = std::flat_set<int, std::less<int>, std::vector<int, A>>;
    auto v = std::vector<int, A>({1,1,1,2,2,3,2,3,3}, A(4));
    auto m = M(v, A(5)); // replaces v's allocator
    assert(!v.empty()); // it was an lvalue above
    assert((m == M{1,2,3}));
    assert(std::move(m).extract().get_allocator() == A(5));
  }
  {
    using A = test_allocator<int>;
    using M = std::flat_set<int, std::less<int>, std::deque<int, A>>;
    auto v = std::deque<int, A>({1,1,1,2,2,3,2,3,3}, A(4));
    M m = { v, A(5) }; // implicit ctor
    assert(!v.empty()); // it was an lvalue above
    assert((m == M{1,2,3}));
    assert(std::move(m).extract().get_allocator() == A(5));
  }
  {
    using M = std::flat_set<int, std::less<int>, std::pmr::vector<int>>;
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::vector<M> vm(&mr);
    std::pmr::vector<int> v = {1,1,1,2,2,3,2,3,3};
    assert(v.get_allocator().resource() != &mr);
    vm.emplace_back(v);
    assert(v.size() == 9); // v's value is unchanged, since it was an lvalue above
    assert((vm[0] == M{1,2,3}));
    assert(std::move(vm[0]).extract().get_allocator().resource() == &mr);
  }
  {
    using M = std::flat_set<int, std::less<int>, std::pmr::vector<int>>;
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::vector<M> vm(&mr);
    std::pmr::vector<int> v = {1,1,1,2,2,3,2,3,3};
    assert(v.get_allocator().resource() != &mr);
    vm.emplace_back(std::move(v));
    assert(v.size() == 9); // v's value is unchanged (before LWG 3802)
    assert((vm[0] == M{1,2,3}));
    assert(std::move(vm[0]).extract().get_allocator().resource() == &mr);
  }
  {
    using M = std::flat_set<int, std::less<int>, std::pmr::vector<int>>;
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::vector<M> vm(&mr);
    std::pmr::vector<int> v({1,1,1,2,2,3,2,3,3}, &mr);
    assert(v.get_allocator().resource() == &mr);
    vm.emplace_back(std::move(v));
    assert(v.size() == 9); // v's value is unchanged (before LWG 3802)
    assert((vm[0] == M{1,2,3}));
    assert(std::move(vm[0]).extract().get_allocator().resource() == &mr);
  }
#if 0 // LWG 3802
  {
    using M = std::flat_set<MoveOnly, std::less<>, std::pmr::vector<MoveOnly>>;
    std::pmr::vector<M> vm;
    std::pmr::vector<MoveOnly> v;
    vm.emplace_back(std::move(v)); // this was a hard error before LWG 3802
    assert(vm.size() == 1);
  }
#endif
  return 0;
}
