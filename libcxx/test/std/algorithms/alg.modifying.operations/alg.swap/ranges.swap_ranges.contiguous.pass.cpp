//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <algorithm>

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<input_iterator I1, sentinel_for<I1> S1, input_iterator I2, sentinel_for<I2> S2>
//   requires indirectly_swappable<I1, I2>
//   constexpr ranges::swap_ranges_result<I1, I2>
//     ranges::swap_ranges(I1 first1, S1 last1, I2 first2, S2 last2);
// template<input_range R1, input_range R2>
//   requires indirectly_swappable<iterator_t<R1>, iterator_t<R2>>
//   constexpr ranges::swap_ranges_result<borrowed_iterator_t<R1>, borrowed_iterator_t<R2>>
//     ranges::swap_ranges(R1&& r1, R2&& r2);

#include <algorithm>
#include <cassert>
#include <ranges>
#include <string>

#include "test_macros.h"

template <class R1, class R2>
constexpr void test_range(const std::string& s1, const std::string& s2, R1&& r1, R2&& r2) {
  static_assert(std::ranges::contiguous_range<R1>);
  static_assert(std::ranges::contiguous_range<R2>);
  assert(std::ranges::distance(r2) < std::ranges::distance(r1));
  // We must never form a pointer to r2[r1.size()], not even when the ranges are contiguous,
  // because r2 is shorter than r1 and so r2[r1.size()] might be UB.
  {
    // Longer string first.
    auto [it1, it2] = std::ranges::swap_ranges(r1, r2);
    assert(it1 == s1.begin() + s2.size());
    assert(it2 == s2.end());
    assert(s1 == "shorterstringwithnospacesinvolvedtedaftertheendofthe second string");
    assert(s2 == "longerstringwiththefirstspaceloca");
  }
  {
    // Shorter string first.
    auto [it2, it1] = std::ranges::swap_ranges(r2, r1);
    assert(it1 == s1.begin() + s2.size());
    assert(it2 == s2.end());
    assert(s1 == "longerstringwiththefirstspacelocatedaftertheendofthe second string");
    assert(s2 == "shorterstringwithnospacesinvolved");
  }
  {
    // Longer string first, iterator/sentinel version.
    auto [it1, it2] = std::ranges::swap_ranges(r1.begin(), r1.end(), r2.begin(), r2.end());
    assert(it1 == s1.begin() + s2.size());
    assert(it2 == s2.end());
    assert(s1 == "shorterstringwithnospacesinvolvedtedaftertheendofthe second string");
    assert(s2 == "longerstringwiththefirstspaceloca");
  }
  {
    // Shorter string first, iterator/sentinel version.
    auto [it2, it1] = std::ranges::swap_ranges(r2.begin(), r2.end(), r1.begin(), r1.end());
    assert(it1 == s1.begin() + s2.size());
    assert(it2 == s2.end());
    assert(s1 == "longerstringwiththefirstspacelocatedaftertheendofthe second string");
    assert(s2 == "shorterstringwithnospacesinvolved");
  }
}

constexpr bool test() {
  auto isntSpace = [](char ch) { return ch != ' '; };
  std::string s1 = "longerstringwiththefirstspacelocatedaftertheendofthe second string";
  std::string s2 = "shorterstringwithnospacesinvolved";
  {
    // The strings themselves.
    test_range(s1, s2, s1, s2);
  }
  {
    // Contiguous, non-common ranges.
    auto v1 = s1 | std::views::take_while(isntSpace);
    auto v2 = s2 | std::views::take_while(isntSpace);
    static_assert(std::same_as<decltype(v1), decltype(v2)>);
    static_assert(!std::ranges::common_range<decltype(v1)>);
    static_assert(!std::ranges::sized_range<decltype(v1)>);
    test_range(s1, s2, v1, v2);
  }
  {
    // Contiguous, common ranges.
    auto v1 = s1 | std::views::take(52);
    auto v2 = s2 | std::views::take(33);
    static_assert(std::same_as<decltype(v1), decltype(v2)>);
    static_assert(std::ranges::contiguous_range<decltype(v1)>);
    static_assert(std::ranges::common_range<decltype(v1)>);
    static_assert(std::ranges::sized_range<decltype(v1)>);
    test_range(s1, s2, v1, v2);
  }
  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;
}
