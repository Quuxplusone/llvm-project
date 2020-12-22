//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <algorithm>

// template<InputIterator InIter, OutputIterator<auto, InIter::reference> OutIter>
//   constexpr OutIter   // constexpr after C++17
//   copy(InIter first, InIter last, OutIter result);

#include <algorithm>
#include <cassert>
#include <iterator>
#include <type_traits>

#include "test_macros.h"

int move_assignments = 0;

struct S {
    int i;
    S() = default;
    S(const S&) = default;
    S& operator=(const S&) = default;
    S& operator=(S&&) { ++move_assignments; return *this; }
};
static_assert(std::is_trivially_copy_assignable<S>::value, "");

int main() {
    S a[2];
    S b[2];
    std::copy(std::make_move_iterator(a), std::make_move_iterator(a+2), b);
    assert(move_assignments == 2);
}

