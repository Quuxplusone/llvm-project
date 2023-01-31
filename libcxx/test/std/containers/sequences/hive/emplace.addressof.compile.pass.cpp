//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// template <class... Args> iterator emplace(Args&&... args);
// template <class... Args> iterator emplace_hint(const_iterator pos, Args&&... args);

// Validate whether the container can be used with an ADL-hijacking operator&

#include <hive>

#include "test_macros.h"
#include "operator_hijacker.h"

void test() {
  std::hive<operator_hijacker> v;
  v.emplace();
  v.emplace_hint(v.end());
}
