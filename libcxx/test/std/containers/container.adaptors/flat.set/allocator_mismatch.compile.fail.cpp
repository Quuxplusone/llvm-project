//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>
//   The container's value type must be the same as the allocator's value type

#include <flat_set>

int main(int, char**)
{
  std::flat_set<int, std::less<int>, std::allocator<long>> s;

  return 0;
}
