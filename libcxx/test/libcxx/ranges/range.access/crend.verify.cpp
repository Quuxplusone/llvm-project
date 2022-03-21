//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::ranges::crend
//   Substitution failure should give reasonably nice diagnostics.

#include <forward_list>
#include <ranges>
#include <string>

void nonclasstype() {
  std::ranges::crend(42);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}

void classtype() {
  struct S {} s;
  std::ranges::crend(s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}

void baditerator() {
  std::forward_list<int> s;
  std::ranges::crend(s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}

void rvalue() {
  std::ranges::crend(std::string());
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'is_lvalue_reference_v<std::string &&>' evaluated to false}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}
