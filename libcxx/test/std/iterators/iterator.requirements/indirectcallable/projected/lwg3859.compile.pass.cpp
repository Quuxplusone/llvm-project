//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// LWG 3859 "std::projected cannot handle proxy iterator"

#include <algorithm>
#include <ranges>
#include <tuple>
#include <vector>

struct GoodCmp {
  bool operator()(std::tuple<int&>, std::tuple<int&>) const;
};

struct EvilCmp {
  bool operator()(std::tuple<int&>, std::tuple<int&>) const;
  bool operator()(auto, auto) const = delete;
};

template<class T, class Compare>
concept HasZipSort = requires (T v, Compare compare) {
  std::ranges::sort(std::views::zip(v), Compare());
};

static_assert(HasZipSort<std::vector<int>, GoodCmp>);
static_assert(!HasZipSort<std::vector<int>, EvilCmp>);
