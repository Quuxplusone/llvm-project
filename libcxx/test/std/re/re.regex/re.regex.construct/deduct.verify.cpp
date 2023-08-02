//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <regex>

// Test CTAD on cases where deduction should fail.

#include <regex>

void test() {
  {
    // Cannot deduce CharT from nothing
    std::basic_regex re;
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}basic_regex'}}
  }
  {
    // basic_regex(ForwardIterator, ForwardIterator)
    // int is not an iterator
    std::basic_regex re(23, 34);
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}basic_regex'}}
  }
  {
    // basic_regex(ForwardIterator, ForwardIterator, flag_type)
    // double is not an iterator
    std::basic_regex re(23.0, 34.0, std::regex_constants::basic);
      // expected-error-re@-1{{no viable constructor or deduction guide for deduction of template arguments of '{{(std::)?}}basic_regex'}}
  }
}
