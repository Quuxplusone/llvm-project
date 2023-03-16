//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// template<class K> iterator insert(K&& x);
// template<class K> iterator insert(const_iterator hint, K&& x);

#include <algorithm>
#include <compare>
#include <concepts>
#include <flat_set>
#include <functional>

#include "test_macros.h"
#include "test_iterators.h"
#include "min_allocator.h"

static int expensive_comparisons = 0;
static int cheap_comparisons = 0;

struct CompareCounter {
  int i_ = 0;
  CompareCounter(int i) : i_(i) {}
  friend auto operator<=>(const CompareCounter& x, const CompareCounter& y) {
    expensive_comparisons += 1;
    return x.i_ <=> y.i_;
  }
  bool operator==(const CompareCounter&) const = default;
  friend auto operator<=>(const CompareCounter& x, int y) {
    cheap_comparisons += 1;
    return x.i_ <=> y;
  }
};

struct Address {
  const char *p_ = nullptr;
  template<class T> Address(T t) : p_((const char*)&*t) {}
  friend bool operator<(Address x, Address y) { return std::less<const char*>()(x.p_, y.p_); }
  friend bool operator==(const Address&, const Address&) = default;
};

int main(int, char**)
{
  const CompareCounter expected[] = {1,2,3,4,5};
  {
    // insert(K&&)
    using M = std::flat_multiset<CompareCounter, std::less<>>;
    M m = {1,2,4,5};
    expensive_comparisons = 0;
    cheap_comparisons = 0;
    std::same_as<M::iterator> auto it = m.insert(3); // conversion happens last
    assert(expensive_comparisons == 0);
    assert(cheap_comparisons >= 2);
    assert(it == m.begin() + 2);
    assert(std::ranges::equal(m, expected));
  }
  {
    // insert(const_iterator, K&&)
    using M = std::flat_multiset<CompareCounter, std::less<>>;
    M m = {1,2,4,5};
    expensive_comparisons = 0;
    cheap_comparisons = 0;
    std::same_as<M::iterator> auto it = m.insert(m.begin(), 3);
    assert(expensive_comparisons == 0);
    assert(cheap_comparisons >= 2);
    assert(it == m.begin() + 2);
    assert(std::ranges::equal(m, expected));
  }
  {
    // insert(value_type&&)
    using M = std::flat_multiset<CompareCounter>;
    M m = {1,2,4,5};
    expensive_comparisons = 0;
    cheap_comparisons = 0;
    std::same_as<M::iterator> auto it = m.insert(3); // conversion happens last
    assert(expensive_comparisons >= 2);
    assert(cheap_comparisons == 0);
    assert(it == m.begin() + 2);
    assert(std::ranges::equal(m, expected));
  }
  {
    // insert(const_iterator, value_type&&)
    using M = std::flat_multiset<CompareCounter>;
    M m = {1,2,4,5};
    expensive_comparisons = 0;
    cheap_comparisons = 0;
    std::same_as<M::iterator> auto it = m.insert(m.begin(), 3);
    assert(expensive_comparisons >= 2);
    assert(cheap_comparisons == 0);
    assert(it == m.begin() + 2);
    assert(std::ranges::equal(m, expected));
  }
  {
    // emplace(Args&&...)
    using M = std::flat_multiset<CompareCounter>;
    M m = {1,2,4,5};
    expensive_comparisons = 0;
    cheap_comparisons = 0;
    std::same_as<M::iterator> auto it = m.emplace(3); // conversion happens first
    assert(expensive_comparisons >= 2);
    assert(cheap_comparisons == 0);
    assert(it == m.begin() + 2);
    assert(std::ranges::equal(m, expected));
  }
  {
    // insert(const_iterator, const_iterator)
    using M = std::flat_multiset<Address, std::less<>>;
    int a, b, c, d, e;
    M m = {&a, &b};
    M m2 = {&d, &e};
    assert(m.size() == 2);
    m.insert(m2.begin()); // insert(K&&)
    ASSERT_SAME_TYPE(decltype(m.insert(m2.begin())), M::iterator);
    assert(m.size() == 3);
    m.insert(m.begin(), &c); // insert(const_iterator, K&&)
    ASSERT_SAME_TYPE(decltype(m.insert(m.begin(), &c)), M::iterator);
    assert(m.size() == 4);
    m.insert(m2.begin(), m2.end()); // insert(const_iterator, const_iterator)
    ASSERT_SAME_TYPE(decltype(m.insert(m2.begin(), m2.end())), void);
    assert(m.size() == 6);
  }
  return 0;
}
