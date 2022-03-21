//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::ranges::distance
//   Substitution failure should give reasonably nice diagnostics.

#include <iterator>

void f() {
  std::ranges::distance(1, 2);
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'sentinel_for<int, int>' evaluated to false}}
  // expected-note@*:* {{because 'int' does not satisfy 'input_or_output_iterator'}}
  // expected-note@*:* {{because '*__i' would be invalid}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'sized_sentinel_for<int, decay_t<int> >' evaluated to false}}
  // expected-note@*:* {{because 'sentinel_for<int, int>' evaluated to false}}
  // expected-note@*:* {{because 'int' does not satisfy 'input_or_output_iterator'}}
  // expected-note@*:* {{because '*__i' would be invalid}}
  // expected-note@*:* {{candidate function template not viable}}
}

void g() {
  int v[10];
  char w[10];
  std::ranges::distance(std::begin(v), std::end(w));
  // expected-error@-1 {{no matching function for call}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'sentinel_for<char *, int *>' evaluated to false}}
  // expected-note@*:* {{because '__weakly_equality_comparable_with<char *, int *>' evaluated to false}}
  // expected-note@*:* {{because '__t == __u' would be invalid}}
  // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
  // expected-note@*:* {{because 'sized_sentinel_for<char *, decay_t<int *> >' evaluated to false}}
  // expected-note@*:* {{because 'sentinel_for<char *, int *>' evaluated to false}}
  // expected-note@*:* {{because '__weakly_equality_comparable_with<char *, int *>' evaluated to false}}
  // expected-note@*:* {{because '__t == __u' would be invalid}}
  // expected-note@*:* {{candidate function template not viable}}
}
