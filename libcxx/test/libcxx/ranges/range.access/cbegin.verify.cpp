//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::ranges::cbegin
//   Substitution failure should give reasonably nice diagnostics.

#include <ranges>
#include <string>

void nonclasstype() {
  std::ranges::cbegin(42);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}

void classtype() {
  struct S {} s;
  std::ranges::cbegin(s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}

void baditerator() {
  struct S { int begin(); } s;
  std::ranges::cbegin(s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}

void rvalue() {
  std::ranges::cbegin(std::string());
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'is_lvalue_reference_v<std::string &&>' evaluated to false}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}
