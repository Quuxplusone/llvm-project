//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::ranges::size
//   Substitution failure should give reasonably nice diagnostics.

#include <ranges>

void f() {
  std::ranges::size(42);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: could not match}}
  // expected-note@*:* {{candidate template ignored: could not match}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'int' does not satisfy '__member_size'}}
  // expected-note@*:* {{because 'int' does not satisfy '__workaround_52970'}}
  // expected-note@*:* {{because 'is_class_v<__remove_cvref_t<int> >' evaluated to false}}
  // expected-note@*:* {{and 'is_union_v<__remove_cvref_t<int> >' evaluated to false}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'int' does not satisfy '__unqualified_size'}}
  // expected-note@*:* {{because 'remove_cvref_t<int>' (aka 'int') does not satisfy '__class_or_enum'}}
  // expected-note@*:* {{because 'is_class_v<int>' evaluated to false}}
  // expected-note@*:* {{and 'is_union_v<int>' evaluated to false}}
  // expected-note@*:* {{and 'is_enum_v<int>' evaluated to false}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}

void g() {
  struct S {} s;
  std::ranges::size(s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: could not match}}
  // expected-note@*:* {{candidate template ignored: could not match}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'S &' does not satisfy '__member_size'}}
  // expected-note@*:* {{because 'static_cast<typename decay<decltype((__t.size()))>::type>(__t.size())' would be invalid}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'S &' does not satisfy '__unqualified_size'}}
  // expected-note@*:* {{because 'static_cast<typename decay<decltype((size(__t)))>::type>(size(__t))' would be invalid}}
  // expected-note@*:* {{candidate template ignored: substitution failure}}
}
