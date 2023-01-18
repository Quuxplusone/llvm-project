//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// Make sure each "transparent comparator" member function
// constructs the actual value_type at the appropriate point
// for that specific member function (if ever).

#include <cassert>
#include <deque>
#include <flat_set>
#include <functional>

#include "test_macros.h"

struct Counter {
  // Unlike in "Counter.h", these counts never go back down.
  Counter(int i) : i_(i) { gConstructed += 1; }
  Counter(Counter&& rhs) noexcept : i_(rhs.i_) { if (i_ == 4) gMoveConstructed += 1; }
  Counter(const Counter& rhs) : i_(rhs.i_) { if (i_ == 4) gCopyConstructed += 1; }
  Counter& operator=(const Counter&) = default;
  Counter& operator=(Counter&&) = default;
  ~Counter() = default;
  auto operator<=>(const Counter&) const = default;
  auto operator<=>(int i) const { return i_ <=> i; }
  bool operator==(int i) const { return i_ == i; }
  int i_;

  static int gConstructed;
  static int gMoveConstructed;
  static int gCopyConstructed;

  static void reset() {
    gConstructed = 0;
    gMoveConstructed = 0;
    gCopyConstructed = 0;
  }
};
int Counter::gConstructed = 0;
int Counter::gMoveConstructed = 0;
int Counter::gCopyConstructed = 0;

void test_insert()
{
  // Inserting at the beginning or end of M won't move any existing elements.
  using M = std::flat_set<Counter, std::less<>, std::deque<Counter>>;
  {
    M m = {1, 2, 3};
    Counter::reset();
    m.insert(Counter(4));
    assert(Counter::gConstructed == 1);
    assert(Counter::gMoveConstructed == 1);
    assert(Counter::gCopyConstructed == 0);
    Counter::reset();
    m.insert(Counter(4));
    assert(Counter::gConstructed == 1);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
  }
  {
    M m = {1, 2, 3};
    Counter lvalue = 4;
    Counter::reset();
    m.insert(lvalue);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 1);
    Counter::reset();
    m.insert(lvalue);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
  }
  {
    M m = {1, 2, 3};
    Counter::reset();
    m.insert(4);
    assert(Counter::gConstructed == 1);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
    Counter::reset();
    m.insert(4);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
  }
}

void test_insert_hint()
{
  using M = std::flat_set<Counter, std::less<>>;
  {
    M m = {1, 2, 3};
    Counter::reset();
    m.insert(m.begin(), Counter(4));
    assert(Counter::gConstructed == 1);
    assert(Counter::gMoveConstructed == 1);
    assert(Counter::gCopyConstructed == 0);
    Counter::reset();
    m.insert(m.begin(), Counter(4));
    assert(Counter::gConstructed == 1);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
  }
  {
    M m = {1, 2, 3};
    Counter lvalue = 4;
    Counter::reset();
    m.insert(m.begin(), lvalue);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 1);
    Counter::reset();
    m.insert(m.begin(), lvalue);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
  }
  {
    M m = {1, 2, 3};
    Counter::reset();
    m.insert(m.begin(), 4);
    assert(Counter::gConstructed == 1);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
    Counter::reset();
    m.insert(m.begin(), 4);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
  }
}

void test_emplace()
{
  using M = std::flat_set<Counter, std::less<>>;
  {
    M m = {1, 2, 3};
    Counter::reset();
    m.emplace(4);
    assert(Counter::gConstructed == 1);
    assert(Counter::gMoveConstructed == 1);
    assert(Counter::gCopyConstructed == 0);
    Counter::reset();
    m.emplace(4);
    assert(Counter::gConstructed == 1);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
  }
}

void test_emplace_hint()
{
  using M = std::flat_set<Counter, std::less<>>;
  {
    M m = {1, 2, 3};
    Counter::reset();
    m.emplace_hint(m.begin(), 4);
    assert(Counter::gConstructed == 1);
    assert(Counter::gMoveConstructed == 1);
    assert(Counter::gCopyConstructed == 0);
    Counter::reset();
    m.emplace_hint(m.begin(), 4);
    assert(Counter::gConstructed == 1);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
  }
}

void test_contains()
{
  using M = std::flat_set<Counter, std::less<>>;
  {
    M m = {1, 2, 3};
    Counter::reset();
    (void)m.contains(Counter(4));
    assert(Counter::gConstructed == 1);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
    Counter::reset();
    (void)m.contains(4);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
    Counter lvalue = 4;
    Counter::reset();
    (void)m.contains(lvalue);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
  }
}

void test_count()
{
  using M = std::flat_set<Counter, std::less<>>;
  {
    M m = {1, 2, 3};
    Counter::reset();
    (void)m.count(Counter(4));
    assert(Counter::gConstructed == 1);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
    Counter::reset();
    (void)m.count(4);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
    Counter lvalue = 4;
    Counter::reset();
    (void)m.count(lvalue);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
  }
}

void test_find()
{
  using M = std::flat_set<Counter, std::less<>>;
  {
    M m = {1, 2, 3};
    Counter::reset();
    (void)m.find(Counter(4));
    assert(Counter::gConstructed == 1);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
    Counter::reset();
    (void)m.find(4);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
    Counter lvalue = 4;
    Counter::reset();
    (void)m.find(lvalue);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
  }
}

void test_lower_bound()
{
  using M = std::flat_set<Counter, std::less<>>;
  {
    M m = {1, 2, 3};
    Counter::reset();
    (void)m.lower_bound(Counter(4));
    assert(Counter::gConstructed == 1);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
    Counter::reset();
    (void)m.lower_bound(4);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
    Counter lvalue = 4;
    Counter::reset();
    (void)m.lower_bound(lvalue);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
  }
}

void test_upper_bound()
{
  using M = std::flat_set<Counter, std::less<>>;
  {
    M m = {1, 2, 3};
    Counter::reset();
    (void)m.upper_bound(Counter(4));
    assert(Counter::gConstructed == 1);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
    Counter::reset();
    (void)m.upper_bound(4);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
    Counter lvalue = 4;
    Counter::reset();
    (void)m.upper_bound(lvalue);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
  }
}

void test_equal_range()
{
  using M = std::flat_set<Counter, std::less<>>;
  {
    M m = {1, 2, 3};
    Counter::reset();
    (void)m.equal_range(Counter(4));
    assert(Counter::gConstructed == 1);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
    Counter::reset();
    (void)m.equal_range(4);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
    Counter lvalue = 4;
    Counter::reset();
    (void)m.equal_range(lvalue);
    assert(Counter::gConstructed == 0);
    assert(Counter::gMoveConstructed == 0);
    assert(Counter::gCopyConstructed == 0);
  }
}

int main(int, char**)
{
  test_insert();
  test_insert_hint();
  test_emplace();
  test_emplace_hint();

  test_contains();
  test_count();
  test_find();
  test_lower_bound();
  test_upper_bound();
  test_equal_range();

  return 0;
}
