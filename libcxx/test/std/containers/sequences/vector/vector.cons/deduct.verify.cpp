//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <vector>

// Test CTAD on cases where deduction should fail.

#include <vector>
#include <memory>

void test() {
  {
    // Cannot deduce T from nothing
    std::vector vec;
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}vector'}}
  }
  {
    // Cannot deduce T from just (Alloc)
    auto vec = std::vector(std::allocator<int>());
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}vector'}}
  }
  {
    // Cannot deduce T from just (Count)
    auto vec = std::vector(42);
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}vector'}}
  }
}
