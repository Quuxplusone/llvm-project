//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <unordered_map>

// Test CTAD on cases where deduction should fail.

#include <unordered_map>
#include <functional>
#include <memory>

void test() {
  using P = std::pair<const int, int>;
  {
    // cannot deduce Key from nothing
    std::unordered_map m;
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'unordered_map'}}
  }
  {
    // cannot deduce Key from just (Size)
    std::unordered_map m(42);
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'unordered_map'}}
  }
  {
    // cannot deduce Key from just (Size, Hash)
    std::unordered_map m(42, std::hash<int>());
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'unordered_map'}}
  }
  {
    // cannot deduce Key from just (Size, Hash, Pred)
    std::unordered_map m(42, std::hash<int>(), std::equal_to<int>());
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'unordered_map'}}
  }
  {
    // cannot deduce Key from just (Size, Hash, Pred, Allocator)
    std::unordered_map m(42, std::hash<int>(), std::equal_to<int>(), std::allocator<P>());
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'unordered_map'}}
  }
  {
    // cannot deduce Key from just (Allocator)
    std::unordered_map m(std::allocator<P>{});
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'unordered_map'}}
  }
  {
    // cannot deduce Key from just (Size, Allocator)
    std::unordered_map m(42, std::allocator<P>());
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'unordered_map'}}
  }
  {
    // cannot deduce Key from just (Size, Hash, Allocator)
    std::unordered_map m(42, std::hash<int>(), std::allocator<P>());
        // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'unordered_map'}}
  }
}
