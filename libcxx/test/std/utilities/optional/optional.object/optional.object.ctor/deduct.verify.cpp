//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <optional>

// Test CTAD on cases where deduction should fail.

#include <optional>

void test() {
  {
    // Cannot deduce T from nothing
    std::optional opt;
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}optional'}}
  }
  {
    // Reject optional<nullopt_t> as ill-formed
    auto opt = std::optional(std::nullopt);
      // expected-error-re@optional:* {{static assertion failed{{.*}}instantiation of optional with nullopt_t is ill-formed}}
  }
}
