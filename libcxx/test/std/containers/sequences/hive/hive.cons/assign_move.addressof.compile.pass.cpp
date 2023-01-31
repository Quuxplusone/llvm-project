//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// hive& operator=(hive&& c);

// Validate whether the operation properly guards against ADL-hijacking operator&

#include <hive>

#include "test_macros.h"
#include "operator_hijacker.h"

void test() {
  std::hive<operator_hijacker> lo;
  std::hive<operator_hijacker> l;
  l = std::move(lo);
}
