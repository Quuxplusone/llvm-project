//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class T>
// concept equality_comparable = // see below

#include <concepts>
#include <type_traits>

template <class T>
concept fake_equality_comparable_with_T_T =
  std::equality_comparable<T> &&
  std::equality_comparable<T> &&
  std::common_reference_with<
    const std::remove_reference_t<T>&,
    const std::remove_reference_t<T>&> &&
  std::equality_comparable<
    std::common_reference_t<
      const std::remove_reference_t<T>&,
      const std::remove_reference_t<T>&>> &&
  std::equality_comparable<T>;  // i.e. WeaklyEqualityComparableWith<T,T>


template<class T> requires fake_equality_comparable_with_T_T<T>
constexpr int f() { return 1; }
template<class T> requires (std::equality_comparable_with<T, T> && true)
constexpr int f() { return 2; }

template<class T> requires (fake_equality_comparable_with_T_T<T> && true)
constexpr int g() { return 2; }
template<class T> requires std::equality_comparable_with<T, T>
constexpr int g() { return 1; }

static_assert(f<int>() == 2);
static_assert(g<int>() == 2);
