//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// iterator erase(const_iterator position);

#include <compare>
#include <concepts>
#include <deque>
#include <flat_set>
#include <functional>
#include <utility>

#include "test_macros.h"
#include "min_allocator.h"

struct KeyConvertibleFromIterator {
  int value_;
  template<class T> KeyConvertibleFromIterator(const T& t) : value_(t) {}
  friend auto operator<=>(KeyConvertibleFromIterator, KeyConvertibleFromIterator) = default;
};

template<class M>
void test_lwg2059() {
  M m = {1,2,3};
  std::same_as<typename M::iterator> auto it = m.find(2);
  assert(it == m.begin() + 1);
  it = m.erase(it); // erase(const_iterator) takes precedence
  assert(it == m.begin() + 1);
  assert((m == M{1,3}));
}

template<class A, class B>
struct ComparableToAConvertibleToB {
  A a_;
  B b_;
  explicit ComparableToAConvertibleToB(A a, B b) : a_(a), b_(b) {}
  auto operator<=>(const A& a) const { return a_ <=> a; }
  operator B() const { return b_; }
};

template<class M>
void test_p2077() {
  // P2077R3: "Additionally, [heterogeneous erase] shall not participate in overload resolution
  // if is_convertible_v<K&&, iterator> || is_convertible_v<K&&, const_iterator>"
  {
    using T = typename M::key_type;
    M m = {T(0), T(1)};
    std::same_as<typename M::iterator> auto it = m.find(T(0));
    assert(it == m.begin());
    auto cti = ComparableToAConvertibleToB<T, typename M::iterator>(T(1), it);
    it = m.erase(cti); // erase(K&&) drops out; erase(const_iterator) is a better match than erase(const T&)
    assert(it == m.begin());
    assert((m == M{T(1)}));
  }
  {
    using T = typename M::key_type;
    M m = {T(0), T(1)};
    std::same_as<typename M::const_iterator> auto it = m.find(T(1));
    assert(it == m.begin() + 1);
    auto cti = ComparableToAConvertibleToB<T, typename M::const_iterator>(T(0), it);
    it = m.erase(cti); // erase(K&&) drops out; erase(const_iterator) is a better match than erase(const T&)
    assert(it == m.end());
    assert((m == M{T(0)}));
  }
}

int main(int, char**)
{
  {
    using M = std::flat_set<int>;
    M m = {1,2,3,4,5,6,7,8};
    assert(m.size() == 8);
    ASSERT_SAME_TYPE(decltype(m.erase(m.begin())), M::iterator);
    ASSERT_SAME_TYPE(decltype(m.erase(m.cbegin())), M::iterator);
    auto it = m.erase(m.cbegin() + 3);
    assert(it == m.cbegin() + 3);
    assert((m == M{1,2,3,5,6,7,8}));

    it = m.erase(m.cbegin());
    assert(it == m.cbegin());
    assert((m == M{2,3,5,6,7,8}));

    it = m.erase(m.cbegin() + 5);
    assert(it == m.end());
    assert((m == M{2,3,5,6,7}));

    it = m.erase(m.cbegin() + 1);
    assert(it == m.cbegin() + 1);
    assert((m == M{2,5,6,7}));
  }
  {
    using M = std::flat_set<int, std::less<int>, std::deque<int, min_allocator<int>>>;
    std::deque<int, min_allocator<int>> container = {3,4};
    container.insert(container.begin(), {1,2}); // now container is [_ 1 2] [3 4 _]
    M m = M(std::sorted_unique, std::move(container));
    ASSERT_SAME_TYPE(decltype(m.erase(m.begin())), M::iterator);
    ASSERT_SAME_TYPE(decltype(m.erase(m.cbegin())), M::iterator);
    auto it = m.erase(m.cbegin() + 1); // shift right: m is [_ _ 1] [3 4 _]
    assert(it == m.cbegin() + 1);
    assert((m == M{1,3,4}));
    it = m.erase(m.cbegin() + 1); // shift left: m is [_ _ 1] [4 _ _]
    assert(it == m.cbegin() + 1);
    assert((m == M{1,4}));
    it = m.erase(m.cbegin());
    assert(it == m.cbegin());
    assert(m == M{4});
    it = m.erase(m.cbegin());
    assert(it == m.cbegin());
    assert(m.empty());
  }
  {
    test_lwg2059< std::flat_set<KeyConvertibleFromIterator> >();
    test_lwg2059< std::flat_set<KeyConvertibleFromIterator, std::less<>> >();
  }
  {
    test_p2077< std::flat_set<int, std::less<>, std::vector<int>> >();
    test_p2077< std::flat_set<int, std::less<>, std::deque<int>> >();
    test_p2077< std::flat_set<bool, std::less<>, std::vector<bool>> >();
    test_p2077< std::flat_set<bool, std::less<>, std::deque<bool>> >();
  }
  return 0;
}
