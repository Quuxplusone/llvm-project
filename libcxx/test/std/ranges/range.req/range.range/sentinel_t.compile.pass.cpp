//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: libcpp-no-concepts
// UNSUPPORTED: libcpp-has-no-incomplete-ranges

// template<range _Rp>
// using sentinel_t = decltype(ranges::end(declval<_Rp&>()));

#include <ranges>
#include <concepts>

struct Range {
    char *begin() &;
    char *end() &;
    short *begin() &&;
    short *end() &&;
    int *begin() const&;
    int *end() const&;
    long *begin() const&&;
    long *end() const&&;
};

static_assert(std::same_as<std::ranges::sentinel_t<Range>, char*>);
static_assert(std::same_as<std::ranges::sentinel_t<Range&>, char*>);
static_assert(std::same_as<std::ranges::sentinel_t<Range&&>, char*>);
static_assert(std::same_as<std::ranges::sentinel_t<const Range>, int*>);
static_assert(std::same_as<std::ranges::sentinel_t<const Range&>, int*>);
static_assert(std::same_as<std::ranges::sentinel_t<const Range&&>, int*>);
static_assert(std::same_as<std::ranges::sentinel_t<int[10]>, int*>);
static_assert(std::same_as<std::ranges::sentinel_t<int(&)[10]>, int*>);
