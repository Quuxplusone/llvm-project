//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <array>

// Test CTAD on cases where deduction should fail.

#include <array>

void test() {
  {
    // Cannot deduce T from nothing
    std::array a;
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}array'}}
  }
  {
    // Cannot deduce T from a mixed initializer list
    std::array arr{1,2,3L};
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}array'}}
  }
}
