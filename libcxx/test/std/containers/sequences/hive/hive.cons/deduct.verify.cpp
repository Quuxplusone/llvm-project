//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// Test CTAD on cases where deduction should fail.

#include <hive>
#include <memory>

void test() {
  {
    // Cannot deduce T from nothing
    std::hive h;
      // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'std::hive'}}
  }
  {
    // Cannot deduce T from just (Alloc)
    auto h = std::hive(std::allocator<int>());
      // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'std::hive'}}
  }
  {
    // Cannot deduce T from just (Count)
    auto h = std::hive(42);
      // expected-error@-1{{no viable constructor or deduction guide for deduction of template arguments of 'std::hive'}}
  }
}
