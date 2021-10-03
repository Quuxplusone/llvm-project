//===---------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===---------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: libcpp-has-no-incomplete-ranges

// <span>

// template <class It>
// constexpr explicit(Extent != dynamic_extent) span(It first, size_type count);
//  If extent is not equal to dynamic_extent, then count shall be equal to extent.
//

#include <span>
#include <cstddef>

template <class T, size_t extent>
std::span<T, extent> createImplicitSpan(T* ptr, size_t len) {
  return {ptr, len}; // expected-error {{chosen constructor is explicit in copy-initialization}}
}

int main(int, char**) {
  // explicit constructor necessary
  int arr[] = {1, 2, 3};
  createImplicitSpan<int, 1>(arr, 1);

  return 0;
}
