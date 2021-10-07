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

#include "test_iterators.h"

template<class T>
concept HasSentinelT = requires {
    typename std::ranges::sentinel_t<T>;
};

static_assert(!HasSentinelT<int[]>);
static_assert(!HasSentinelT<int(&)[]>);
static_assert(std::same_as<std::ranges::sentinel_t<int[10]>, int*>);
static_assert(std::same_as<std::ranges::sentinel_t<int(&)[10]>, int*>);

struct Range {
    char *begin() &;
    sentinel_wrapper<char*> end() &;
    short *begin() &&;
    sentinel_wrapper<short*> end() &&;
    int *begin() const&;
    sentinel_wrapper<int*> end() const&;
    long *begin() const&&;
    sentinel_wrapper<long*> end() const&&;
};
static_assert(std::same_as<std::ranges::sentinel_t<Range>, sentinel_wrapper<char*>>);
static_assert(std::same_as<std::ranges::sentinel_t<Range&>, sentinel_wrapper<char*>>);
static_assert(std::same_as<std::ranges::sentinel_t<Range&&>, sentinel_wrapper<char*>>);
static_assert(std::same_as<std::ranges::sentinel_t<const Range>, sentinel_wrapper<int*>>);
static_assert(std::same_as<std::ranges::sentinel_t<const Range&>, sentinel_wrapper<int*>>);
static_assert(std::same_as<std::ranges::sentinel_t<const Range&&>, sentinel_wrapper<int*>>);

struct NonConstRange {
    int *begin();
    int *end();
};
static_assert(std::same_as<std::ranges::sentinel_t<NonConstRange>, int*>);
static_assert(std::same_as<std::ranges::sentinel_t<NonConstRange&>, int*>);
static_assert(std::same_as<std::ranges::sentinel_t<NonConstRange&&>, int*>);
static_assert(!HasSentinelT<const NonConstRange>);
static_assert(!HasSentinelT<const NonConstRange&>);
static_assert(!HasSentinelT<const NonConstRange&&>);
