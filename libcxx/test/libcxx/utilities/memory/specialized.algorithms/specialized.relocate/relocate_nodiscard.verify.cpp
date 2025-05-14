//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <memory>

// template <class T>
// [[nodiscard]] T relocate(T *source);

#include <list>
#include <memory>

void test(std::unique_ptr<int> *p, std::list<int> *q, int *r) {
  std::relocate(p); // trivially relocatable type (the magic overload)
    // expected-warning@-1{{ignoring return value of function declared with 'nodiscard' attribute}}
  std::relocate(q); // non-trivially relocatable type (the non-magic overload)
    // expected-warning@-1{{ignoring return value of function declared with 'nodiscard' attribute}}
  std::relocate(r); // trivial-abi type (the non-magic overload)
    // expected-warning@-1{{ignoring return value of function declared with 'nodiscard' attribute}}
}
