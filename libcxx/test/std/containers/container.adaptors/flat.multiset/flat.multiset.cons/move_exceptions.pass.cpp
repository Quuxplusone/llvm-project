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

// flat_multiset(flat_multiset&& s);
// If any member function in [flat.multiset.defn] exits via an exception, the invariant is restored.

#include <algorithm>
#include <cassert>
#include <flat_set>
#include <functional>
#include <utility>

#include "test_macros.h"
#include "MinSequenceContainer.h"

static int countdown = 0;

struct EvilContainer : MinSequenceContainer<int> {
  EvilContainer() = default;
  EvilContainer(EvilContainer&& rhs) {
    // Throw on move-construction.
    if (--countdown == 0) {
      rhs.insert(rhs.end(), 0);
      rhs.insert(rhs.end(), 0);
      throw 42;
    }
  }
};

int main(int, char**)
{
  {
    using M = std::flat_multiset<int, std::less<int>, EvilContainer>;
    M mo = {1, 2, 3};
    countdown = 1;
    try {
      M m = std::move(mo);
      assert(false); // not reached
    } catch (int x) {
      assert(x == 42);
    }
    // The source flat_multiset maintains its class invariant.
    assert(std::is_sorted(mo.begin(), mo.end()));
    LIBCPP_ASSERT(mo.empty());
  }
  return 0;
}
