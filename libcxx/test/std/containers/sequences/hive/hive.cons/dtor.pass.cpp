//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// ~hive()

#include <hive>
#include <cassert>
#include <set>

#include "test_macros.h"


std::set<int> destroyed;

struct Foo {
  explicit Foo(int i) : value(i) { }
  ~Foo() { destroyed.insert(value); }
  int value;
};

int main(int, char**)
{
  {
    std::hive<Foo> hive;
    hive.emplace_back(1);
    hive.emplace_back(2);
    hive.emplace_back(3);
    assert(destroyed.empty());
  }
  assert(destroyed.count(1) == 1);
  assert(destroyed.count(2) == 1);
  assert(destroyed.count(3) == 1);

  return 0;
}
