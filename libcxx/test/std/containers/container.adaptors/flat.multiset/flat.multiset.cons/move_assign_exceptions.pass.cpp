//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20
// UNSUPPORTED: no-exceptions

// <flat_set>

// flat_multiset& operator=(flat_multiset&&);
// If any member function in [flat.multiset.defn] exits via an exception, the invariant is restored.

#include <algorithm>
#include <cassert>
#include <compare>
#include <flat_set>
#include <functional>
#include <utility>
#include <vector>

#include "test_macros.h"

struct Counter {
  int c1, c2, throws;
  void tick() {
    c1 -= 1;
    if (c1 == 0) {
      c1 = c2;
      throws += 1;
      throw 42;
    }
  }
};
Counter g_counter = {0,0,0};

struct ThrowingAssignment {
  ThrowingAssignment(int i) : i_(i) {}
  ThrowingAssignment(const ThrowingAssignment&) = default;
  ThrowingAssignment& operator=(const ThrowingAssignment& rhs) {
    g_counter.tick();
    i_ = rhs.i_;
    g_counter.tick();
    return *this;
  }
  operator int() const { return i_; }
  int i_;
};

struct ThrowingComparator {
  bool operator()(const ThrowingAssignment& a, const ThrowingAssignment& b) const {
    g_counter.tick();
    return a.i_ < b.i_;
  }
};

int main(int, char**)
{
  {
    const int expected[] = {1,2,3,4,5,6,7,8};
    using M = std::flat_multiset<ThrowingAssignment, ThrowingComparator, std::vector<ThrowingAssignment>>;
    for (int first_throw = 1; first_throw < 99; ++first_throw) {
      for (int second_throw = 1; second_throw < 99; ++second_throw) {
        g_counter = {0,0,0};
        M m = {1,2,3,4,5,6,7,8};
        M m2 = {1,2,3};
        try {
          g_counter = {first_throw, second_throw, 0};
          m2 = std::move(m);
          // If it didn't throw at all, we're done.
          g_counter = {0,0,0};
          assert(std::equal(m2.begin(), m2.end(), expected, expected+8));
          LIBCPP_ASSERT(m.empty());
          first_throw = 99;  // "done"
          assert(std::is_sorted(m.begin(), m.end())); // still sorted
          assert(std::is_sorted(m2.begin(), m2.end())); // still sorted
          break;
        } catch (int ex) {
          assert(ex == 42);
          assert(std::is_sorted(m.begin(), m.end())); // still sorted
          LIBCPP_ASSERT(m.empty() || std::equal(m.begin(), m.end(), expected, expected+8));
          assert(std::is_sorted(m2.begin(), m2.end())); // still sorted
          LIBCPP_ASSERT(m2.empty() || std::equal(m2.begin(), m2.end(), expected, expected+8));
          if (g_counter.throws == 1) {
            // We reached the first throw but not the second throw.
            break;
          }
        }
      }
    }
  }
  return 0;
}
