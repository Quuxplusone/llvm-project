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
  // expected-error@-1 {{call to deleted function call operator}}
  // expected-note@*:* {{candidate function [with auto:1 = int] has been explicitly deleted}}
  // expected-note@*:* {{candidate template ignored: could not match}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'int' does not satisfy '__member_end'}}
  // expected-note@*:* {{because 'int' does not satisfy '__can_borrow'}}
  // expected-note@*:* {{because 'is_lvalue_reference_v<int>' evaluated to false}}
  // expected-note@*:* {{and 'enable_borrowed_range<remove_cvref_t<int> >' evaluated to false}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'int' does not satisfy '__unqualified_end'}}
  // expected-note@*:* {{because 'int' does not satisfy '__can_borrow'}}
  // expected-note@*:* {{because 'is_lvalue_reference_v<int>' evaluated to false}}
  // expected-note@*:* {{and 'enable_borrowed_range<remove_cvref_t<int> >' evaluated to false}}
  // expected-error@-14 {{attempt to use a deleted function}}
  // expected-note@*:* {{'operator()<int>' has been explicitly marked deleted here}}
}

void classtype() {
  struct S {} s;
  std::ranges::end(s);
  // expected-error@-1 {{call to deleted function call operator}}
  // expected-note@*:* {{candidate function [with auto:1 = S &] has been explicitly deleted}}
  // expected-note@*:* {{candidate template ignored: could not match}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'S &' does not satisfy '__member_end'}}
  // expected-note@*:* {{because 'typename iterator_t<_Tp>' would be invalid}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'S &' does not satisfy '__unqualified_end'}}
  // expected-note@*:* {{because 'typename iterator_t<_Tp>' would be invalid}}
  // expected-error@-10 {{attempt to use a deleted function}}
  // expected-note@*:* {{'operator()<S &>' has been explicitly marked deleted here}}
}

void baditerator() {
  struct S { int *begin(); int end(); } s;
  std::ranges::end(s);
  // expected-error@-1 {{call to deleted function call operator}}
  // expected-note@*:* {{candidate function [with auto:1 = S &] has been explicitly deleted}}
  // expected-note@*:* {{candidate template ignored: could not match}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'S &' does not satisfy '__member_end'}}
  // expected-note@*:* {{because type constraint 'sentinel_for<int, iterator_t<S &> >' was not satisfied}}
  // expected-note@*:* {{because '__weakly_equality_comparable_with<int, int *>' evaluated to false}}
  // expected-note@*:* {{because '__t == __u' would be invalid}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'S &' does not satisfy '__unqualified_end'}}
  // expected-note@*:* {{because 'static_cast<typename decay<decltype((end(__t)))>::type>(end(__t))' would be invalid}}
  // expected-error@-12 {{attempt to use a deleted function}}
  // expected-note@*:* {{'operator()<S &>' has been explicitly marked deleted here}}
}

void rvalue() {
  std::ranges::end(std::string());
  // expected-error@-1 {{call to deleted function call operator}}
  // expected-note@*:* {{candidate function [with auto:1 = std::string] has been explicitly deleted}}
  // expected-note@*:* {{candidate template ignored: could not match}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'std::string' does not satisfy '__member_end'}}
  // expected-note@*:* {{because 'std::string' does not satisfy '__can_borrow'}}
  // expected-note@*:* {{because 'is_lvalue_reference_v<std::string>' evaluated to false}}
  // expected-note@*:* {{and 'enable_borrowed_range<remove_cvref_t<string> >' evaluated to false}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'std::string' does not satisfy '__unqualified_end'}}
  // expected-note@*:* {{because 'std::string' does not satisfy '__can_borrow'}}
  // expected-note@*:* {{because 'is_lvalue_reference_v<std::string>' evaluated to false}}
  // expected-note@*:* {{and 'enable_borrowed_range<remove_cvref_t<string> >' evaluated to false}}
  // expected-error@-14 {{attempt to use a deleted function}}
  // expected-note@*:* {{'operator()<std::string>' has been explicitly marked deleted here}}
}
