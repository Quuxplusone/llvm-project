//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::ranges::rend
//   Substitution failure should give reasonably nice diagnostics.

#include <forward_list>
#include <ranges>
#include <string>

void nonclasstype() {
  std::ranges::rend(42);
  // expected-error@-1 {{call to deleted function call operator}}
  // expected-note@*:* {{candidate function [with auto:1 = int] has been explicitly deleted}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'int' does not satisfy '__member_rend'}}
  // expected-note@*:* {{because 'int' does not satisfy '__can_borrow'}}
  // expected-note@*:* {{because 'is_lvalue_reference_v<int>' evaluated to false}}
  // expected-note@*:* {{and 'enable_borrowed_range<remove_cvref_t<int> >' evaluated to false}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'int' does not satisfy '__unqualified_rend'}}
  // expected-note@*:* {{because 'int' does not satisfy '__can_borrow'}}
  // expected-note@*:* {{because 'is_lvalue_reference_v<int>' evaluated to false}}
  // expected-note@*:* {{and 'enable_borrowed_range<remove_cvref_t<int> >' evaluated to false}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'int' does not satisfy '__can_reverse'}}
  // expected-note@*:* {{because 'int' does not satisfy '__can_borrow'}}
  // expected-note@*:* {{because 'is_lvalue_reference_v<int>' evaluated to false}}
  // expected-note@*:* {{and 'enable_borrowed_range<remove_cvref_t<int> >' evaluated to false}}
  // expected-error@-18 {{attempt to use a deleted function}}
  // expected-note@*:* {{'operator()<int>' has been explicitly marked deleted here}}
}

void classtype() {
  struct S {} s;
  std::ranges::rend(s);
  // expected-error@-1 {{call to deleted function call operator}}
  // expected-note@*:* {{candidate function [with auto:1 = S &] has been explicitly deleted}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'S &' does not satisfy '__member_rend'}}
  // expected-note@*:* {{because 'ranges::rbegin(__t)' would be invalid}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'S &' does not satisfy '__unqualified_rend'}}
  // expected-note@*:* {{because 'ranges::rbegin(__t)' would be invalid}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'S &' does not satisfy '__can_reverse'}}
  // expected-note@*:* {{because 'ranges::begin(__t)' would be invalid}}
  // expected-error@-12 {{attempt to use a deleted function}}
  // expected-note@*:* {{'operator()<S &>' has been explicitly marked deleted here}}
}

void baditerator() {
  struct S { int *begin(); int end(); } s;
  std::ranges::rend(s);
  // expected-error@-1 {{call to deleted function call operator}}
  // expected-note@*:* {{candidate function [with auto:1 = S &] has been explicitly deleted}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'S &' does not satisfy '__member_rend'}}
  // expected-note@*:* {{because 'ranges::rbegin(__t)' would be invalid}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'S &' does not satisfy '__unqualified_rend'}}
  // expected-note@*:* {{because 'ranges::rbegin(__t)' would be invalid}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'S &' does not satisfy '__can_reverse'}}
  // expected-note@*:* {{because 'same_as<expr-type, decltype(ranges::end(__t))>' would be invalid}}
  // expected-error@-12 {{attempt to use a deleted function}}
  // expected-note@*:* {{'operator()<S &>' has been explicitly marked deleted here}}
}

void rvalue() {
  std::ranges::rend(std::string());
  // expected-error@-1 {{call to deleted function call operator}}
  // expected-note@*:* {{candidate function [with auto:1 = std::string] has been explicitly deleted}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'std::string' does not satisfy '__member_rend'}}
  // expected-note@*:* {{because 'std::string' does not satisfy '__can_borrow'}}
  // expected-note@*:* {{because 'is_lvalue_reference_v<std::string>' evaluated to false}}
  // expected-note@*:* {{and 'enable_borrowed_range<remove_cvref_t<string> >' evaluated to false}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'std::string' does not satisfy '__unqualified_rend'}}
  // expected-note@*:* {{because 'std::string' does not satisfy '__can_borrow'}}
  // expected-note@*:* {{because 'is_lvalue_reference_v<std::string>' evaluated to false}}
  // expected-note@*:* {{and 'enable_borrowed_range<remove_cvref_t<string> >' evaluated to false}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'std::string' does not satisfy '__can_reverse'}}
  // expected-note@*:* {{because 'std::string' does not satisfy '__can_borrow'}}
  // expected-note@*:* {{because 'is_lvalue_reference_v<std::string>' evaluated to false}}
  // expected-note@*:* {{and 'enable_borrowed_range<remove_cvref_t<string> >' evaluated to false}}
  // expected-error@-18 {{attempt to use a deleted function}}
  // expected-note@*:* {{'operator()<std::string>' has been explicitly marked deleted here}}
}
