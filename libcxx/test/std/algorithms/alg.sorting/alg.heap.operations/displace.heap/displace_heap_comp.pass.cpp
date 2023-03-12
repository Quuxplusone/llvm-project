//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20, c++23

// <algorithm>

// template<RandomAccessIterator Iter>
//   requires ShuffleIterator<Iter> && LessThanComparable<Iter::value_type>
//   constexpr Iter::value_type
//   displace_heap(Iter first, Iter last, Compare comp);

#include <algorithm>
#include <cassert>
#include <functional>

#include "test_macros.h"
#include "test_iterators.h"
#include "MoveOnly.h"

template<class T, class Iter>
TEST_CONSTEXPR_CXX26 bool test()
{
    T input[] = {1, 2, 5, 4, 3};
    assert(std::is_heap(input, input + 5, std::greater<T>()));
    T val = std::displace_heap(Iter(input), Iter(input + 5), std::greater<T>()); assert(val == 1);
    val = std::displace_heap(Iter(input), Iter(input + 4), std::greater<T>()); assert(val == 2);
    val = std::displace_heap(Iter(input), Iter(input + 3), std::greater<T>()); assert(val == 3);
    val = std::displace_heap(Iter(input), Iter(input + 2), std::greater<T>()); assert(val == 4);
    val = std::displace_heap(Iter(input), Iter(input + 1), std::greater<T>()); assert(val == 5);
    return true;
}

int main(int, char**)
{
    test<int, random_access_iterator<int*> >();
    test<int, int*>();

    test<MoveOnly, random_access_iterator<MoveOnly*>>();
    test<MoveOnly, MoveOnly*>();

    static_assert(test<int, random_access_iterator<int*>>());
    static_assert(test<int, int*>());
    static_assert(test<MoveOnly, random_access_iterator<MoveOnly*>>());
    static_assert(test<MoveOnly, MoveOnly*>());

    return 0;
}
