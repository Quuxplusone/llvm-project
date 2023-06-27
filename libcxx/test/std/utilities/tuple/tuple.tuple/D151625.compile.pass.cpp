//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Make sure that it's possible to instantiate std::tuple<H*>
// even when H is incomplete.

// UNSUPPORTED: c++03

#include <tuple>

template <class T> struct Holder { T v; };
struct Incomplete;
using H = Holder<Incomplete>;

std::tuple<H&> ident(std::tuple<H&> t);

std::tuple<H*> ident(std::tuple<H*> t) {
  return t;
}

void test() {
  std::tuple<H*, H*> t1 = {nullptr, nullptr};
  auto t2 = std::make_tuple((H*)nullptr, (H*)nullptr);
  auto t3 = std::tuple_cat(t1, t2);
  (void)t3;
}
