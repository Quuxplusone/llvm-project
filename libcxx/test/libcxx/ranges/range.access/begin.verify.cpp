//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::ranges::begin
//   Substitution failure should give reasonably nice diagnostics.

#include <ranges>
#include <string>

void nonclasstype() {
  std::ranges::begin(42);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'remove_cvref_t<int>' (aka 'int') does not satisfy '__class_or_enum'}}
  // expected-note@*:* {{because '_Bool(is_class_v<int> || is_union_v<int> || is_enum_v<int>)' evaluated to false}}
  // expected-note@*:* {{and 'is_array_v<remove_reference_t<int> >' evaluated to false}}
}

void classtype() {
  struct S {} s;
  std::ranges::begin(s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because '__t.begin()' would be invalid}}
  // expected-note@*:* {{and 'begin(__t)' would be invalid}}
  // expected-note@*:* {{and 'is_array_v<remove_reference_t<S &> >' evaluated to false}}
}

void baditerator() {
  struct S { int begin(); } s;
  std::ranges::begin(s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because '::std::__decay_t<decltype((__t.begin()))>' (aka 'int') does not satisfy 'input_or_output_iterator'}}
  // expected-note@*:* {{because '*__i' would be invalid}}
  // expected-note@*:* {{and 'begin(__t)' would be invalid}}
  // expected-note@*:* {{and 'is_array_v<remove_reference_t<S &> >' evaluated to false}}
}

void rvalue() {
  std::ranges::begin(std::string());
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because '__go(std::forward<_Tp>(__t), __priority_tag<3>())' would be invalid}}
}
