//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// flat_multiset& operator=(flat_multiset&&);
// Preserves the class invariant for the moved-from flat_multiset.

#include <algorithm>
#include <cassert>
#include <compare>
#include <flat_set>
#include <functional>
#include <utility>

#include "test_macros.h"
#include "../../NaiveStaticVector.h"

struct MoveNegates {
    int value_ = 0;
    MoveNegates() = default;
    MoveNegates(int v) : value_(v) {}
    MoveNegates(MoveNegates&& rhs) : value_(rhs.value_) { rhs.value_ = -rhs.value_; }
    MoveNegates& operator=(MoveNegates&& rhs) { value_ = rhs.value_; rhs.value_ = -rhs.value_; return *this; }
    ~MoveNegates() = default;
    auto operator<=>(const MoveNegates&) const = default;
};

struct MoveClears {
    int value_ = 0;
    MoveClears() = default;
    MoveClears(int v) : value_(v) {}
    MoveClears(MoveClears&& rhs) : value_(rhs.value_) { rhs.value_ = 0; }
    MoveClears& operator=(MoveClears&& rhs) { value_ = rhs.value_; rhs.value_ = 0; return *this; }
    ~MoveClears() = default;
    auto operator<=>(const MoveClears&) const = default;
};

int main(int, char**)
{
  {
    const int expected[] = {1,2,3,4,5,6,7,8};
    using M = std::flat_multiset<MoveNegates, std::less<MoveNegates>, NaiveStaticVector<MoveNegates, 10>>;
    M m = M(expected, expected + 8);
    M m2 = M(expected, expected + 3);

    m2 = std::move(m);

    assert(std::equal(m2.begin(), m2.end(), expected, expected+8));
    LIBCPP_ASSERT(m.empty());
    assert(std::is_sorted(m.begin(), m.end())); // still sorted
    m.insert(1);
    m.insert(2);
    assert(m.contains(1));
    assert(m.find(2) != m.end());
  }
  {
    const int expected[] = {1,2,3,4,5,6,7,8};
    using M = std::flat_multiset<MoveClears, std::less<MoveClears>, NaiveStaticVector<MoveClears, 10>>;
    M m = M(expected, expected + 8);
    M m2 = M(expected, expected + 3);

    m2 = std::move(m);

    assert(std::equal(m2.begin(), m2.end(), expected, expected+8));
    LIBCPP_ASSERT(m.empty());
    assert(std::is_sorted(m.begin(), m.end())); // still sorted
    m.insert(1);
    m.insert(2);
    assert(m.contains(1));
    assert(m.find(2) != m.end());
  }
  return 0;
}
