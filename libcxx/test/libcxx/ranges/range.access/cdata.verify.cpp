//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::ranges::cdata
//   Substitution failure should give reasonably nice diagnostics.

#include <ranges>

void f() {
  std::ranges::cdata(42);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}

void g() {
  struct S {} s;
  std::ranges::cdata(s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}
