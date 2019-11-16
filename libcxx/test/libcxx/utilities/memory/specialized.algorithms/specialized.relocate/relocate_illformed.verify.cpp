//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <memory>

// template <class T>
// [[nodiscard]] T relocate(T *source);
//
// This function, being "equivalent to" a move and destroy, should be ill-formed
// whenever T isn't actually move-constructible.

#include <memory>
#include <type_traits>

struct A {
    const std::unique_ptr<int> m;
};
static_assert(std::is_trivially_relocatable<A>::value, "");
static_assert(!std::is_move_constructible<A>::value, "");

void test1(A *p) {
  (void)std::relocate(p);
    // expected-error@*:*{{return std::move(source) must be well-formed}}
}

std::unique_ptr<int> test2(const std::unique_ptr<int>& p) {
  return std::relocate(&p);
    // expected-error@*:*{{return std::move(source) must be well-formed}}
}
