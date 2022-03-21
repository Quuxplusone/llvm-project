//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::ranges::end
//   Substitution failure should give reasonably nice diagnostics.

#include <ranges>
#include <string>

void nonclasstype() {
  std::ranges::end(42);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'remove_cvref_t<int>' (aka 'int') does not satisfy '__class_or_enum'}}
  // expected-note@*:* {{because '_Bool(is_class_v<int> || is_union_v<int> || is_enum_v<int>)' evaluated to false}}
  // expected-note@*:* {{and 'is_bounded_array_v<remove_reference_t<int> >' evaluated to false}}
}

void classtype() {
  struct S {} s;
  std::ranges::end(s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because '__t.end()' would be invalid}}
  // expected-note@*:* {{and 'end(__t)' would be invalid}}
  // expected-note@*:* {{and 'is_bounded_array_v<remove_reference_t<S &> >' evaluated to false}}
}

void baditerator() {
  struct S { int *begin(); int end(); } s;
  std::ranges::end(s);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because type constraint 'sentinel_for<int, iterator_t<remove_reference_t<S &> > >' was not satisfied}}
  // expected-note@*:* {{because '__weakly_equality_comparable_with<int, int *>' evaluated to false}}
  // expected-note@*:* {{because '__t == __u' would be invalid}}
  // expected-note@*:* {{and 'end(__t)' would be invalid}}
  // expected-note@*:* {{and 'is_bounded_array_v<remove_reference_t<S &> >' evaluated to false}}
}

void rvalue() {
  std::ranges::end(std::string());
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because '__go(std::forward<_Tp>(__t), __priority_tag<4>())' would be invalid}}
}
