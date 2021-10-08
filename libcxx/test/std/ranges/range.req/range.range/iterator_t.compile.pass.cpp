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
// using iterator_t = decltype(ranges::begin(declval<_Rp&>()));

#include <ranges>

#include "test_iterators.h"

template<class T>
concept HasIteratorT = requires {
    typename std::ranges::iterator_t<T>;
};

static_assert(std::same_as<std::ranges::iterator_t<int[]>, int*>);
static_assert(std::same_as<std::ranges::iterator_t<int(&)[]>, int*>);
static_assert(std::same_as<std::ranges::iterator_t<int[10]>, int*>);
static_assert(std::same_as<std::ranges::iterator_t<int(&)[10]>, int*>);

struct Range {
    char *begin() &;
    sentinel<char*> end() &;
    short *begin() &&;
    sentinel<short*> end() &&;
    int *begin() const&;
    sentinel<int*> end() const&;
    long *begin() const&&;
    sentinel<long*> end() const&&;
};
static_assert(std::same_as<std::ranges::iterator_t<Range>, char*>);
static_assert(std::same_as<std::ranges::iterator_t<Range&>, char*>);
static_assert(std::same_as<std::ranges::iterator_t<Range&&>, char*>);
static_assert(std::same_as<std::ranges::iterator_t<const Range>, int*>);
static_assert(std::same_as<std::ranges::iterator_t<const Range&>, int*>);
static_assert(std::same_as<std::ranges::iterator_t<const Range&&>, int*>);

struct NonConstRange {
    int *begin();
    int *end();
};
static_assert(std::same_as<std::ranges::iterator_t<NonConstRange>, int*>);
static_assert(std::same_as<std::ranges::iterator_t<NonConstRange&>, int*>);
static_assert(std::same_as<std::ranges::iterator_t<NonConstRange&&>, int*>);
static_assert(!HasIteratorT<const NonConstRange>);
static_assert(!HasIteratorT<const NonConstRange&>);
static_assert(!HasIteratorT<const NonConstRange&&>);
