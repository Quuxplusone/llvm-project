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

// template<range R>
// using range_difference_t = iter_difference_t<iterator_t<R>>;

// template<range R>
// using range_value_t = iter_value_t<iterator_t<R>>;

// template<range R>
// using range_reference_t = iter_reference_t<iterator_t<R>>;

// template<range R>
// using range_rvalue_reference_t = iter_rvalue_reference_t<iterator_t<R>>;

#include <ranges>

struct Range {
    int *begin();
    int *end();
};
static_assert(std::same_as<std::ranges::range_difference_t<Range>, std::ptrdiff_t>);
static_assert(std::same_as<std::ranges::range_value_t<Range>, int>);
static_assert(std::same_as<std::ranges::range_reference_t<Range>, int&>);
static_assert(std::same_as<std::ranges::range_rvalue_reference_t<Range>, int&&>);
