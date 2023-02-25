//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <algorithm>

// template<ForwardIterator Iter1, ForwardIterator Iter2>
//   requires HasSwap<Iter1::reference, Iter2::reference>
//   Iter2 swap_ranges(Iter1 first1, Iter1 last1, Iter2 first2);

#include <algorithm>
#include <cassert>
#include <ranges>
#include <string>

#include "test_macros.h"

template <class It1, class It2>
TEST_CONSTEXPR_CXX20 void
test_range(const std::string& s1, const std::string& s2, It1 first1, It1 last1, It2 first2, It2 last2) {
  assert(std::distance(first2, last2) < std::distance(first1, last1));
  {
    // Shorter string always comes first.
    It1 it1 = std::swap_ranges(first2, last2, first1);
    assert(std::addressof(*it1) == &s1[s2.size()]);
    assert(s1 == "shorterstringwithnospacesinvolvedtedaftertheendofthe second string");
    assert(s2 == "longerstringwiththefirstspaceloca");
  }
  {
    // Swap them back.
    It1 it1 = std::swap_ranges(first2, last2, first1);
    assert(std::addressof(*it1) == &s1[s2.size()]);
    assert(s1 == "longerstringwiththefirstspacelocatedaftertheendofthe second string");
    assert(s2 == "shorterstringwithnospacesinvolved");
  }
}

bool isntSpace(char ch) { return ch != ' '; };

TEST_CONSTEXPR_CXX20 bool test() {
  std::string s1 = "longerstringwiththefirstspacelocatedaftertheendofthe second string";
  std::string s2 = "shorterstringwithnospacesinvolved";
  {
    // The strings themselves.
    test_range(s1, s2, s1.begin(), s1.end(), s2.begin(), s2.end());
  }
#if TEST_STD_VER >= 20
  if (!std::is_constant_evaluated()) {
    // Contiguous, non-common ranges.
    auto bv1 = s1 | std::views::take_while(isntSpace);
    auto bv2 = s2 | std::views::take_while(isntSpace);
    static_assert(std::same_as<decltype(bv1), decltype(bv2)>);
    static_assert(!std::ranges::common_range<decltype(bv1)>);
    static_assert(!std::ranges::sized_range<decltype(bv1)>);
    auto v1 = bv1 | std::views::common;
    auto v2 = bv2 | std::views::common;
    test_range(s1, s2, v1.begin(), v1.end(), v2.begin(), v2.end());
  }
  {
    // Contiguous, common ranges.
    auto v1 = s1 | std::views::take(52);
    auto v2 = s2 | std::views::take(33);
    static_assert(std::same_as<decltype(v1), decltype(v2)>);
    static_assert(std::ranges::contiguous_range<decltype(v1)>);
    static_assert(std::ranges::common_range<decltype(v1)>);
    static_assert(std::ranges::sized_range<decltype(v1)>);
    test_range(s1, s2, v1.begin(), v1.end(), v2.begin(), v2.end());
  }
#endif
  return true;
}

int main(int, char**) {
  test();
#if TEST_STD_VER >= 20
  static_assert(test());
#endif

  return 0;
}
