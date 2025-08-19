//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_map>

// class flat_map
// class flat_multimap

// Extension: SCARY/N2913 iterator compatibility between flat_map and flat_multimap

#include <flat_map>

#include "test_macros.h"

void test() {
  typedef std::flat_map<int, int> M1;
  typedef std::flat_multimap<int, int> M2;

  ASSERT_SAME_TYPE(M1::iterator, M2::iterator);
  ASSERT_SAME_TYPE(M1::const_iterator, M2::const_iterator);
}
