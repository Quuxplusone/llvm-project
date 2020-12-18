//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <algorithm>

// template<RandomAccessIterator Iter>
//   requires ShuffleIterator<Iter> && LessThanComparable<Iter::value_type>
//   constexpr void  // constexpr in C++20
//   pop_heap(Iter first, Iter last, Compare comp);

#include <algorithm>
#include <cassert>
#include <functional>

#include "test_macros.h"
#include "test_iterators.h"
#include "MoveOnly.h"

template<class T, class Iter>
TEST_CONSTEXPR_CXX20 bool test()
{
    T orig[15] = {1,1,2,3,3, 8,4,6,5,5, 5,9,9,7,9};
    T work[15] = {1,1,2,3,3, 8,4,6,5,5, 5,9,9,7,9};
    assert(std::is_heap(orig, orig+15, std::greater<T>()));
    for (int i = 15; i >= 1; --i) {
        std::pop_heap(Iter(work), Iter(work+i), std::greater<T>());
        assert(std::is_heap(work, work+i-1, std::greater<T>()));
        assert(std::min_element(work, work+i-1) == work);
        assert(std::is_permutation(work, work+15, orig));
    }
    assert(std::is_sorted(work, work+15, std::greater<T>()));
    return true;
}

int main(int, char**)
{
    test<int, random_access_iterator<int*> >();
    test<int, int*>();

#if TEST_STD_VER >= 11
    test<MoveOnly, random_access_iterator<MoveOnly*>>();
    test<MoveOnly, MoveOnly*>();
#endif

#if TEST_STD_VER >= 20
    static_assert(test<int, random_access_iterator<int*>>());
    static_assert(test<int, int*>());
    static_assert(test<MoveOnly, random_access_iterator<MoveOnly*>>());
    static_assert(test<MoveOnly, MoveOnly*>());
#endif

    return 0;
}
