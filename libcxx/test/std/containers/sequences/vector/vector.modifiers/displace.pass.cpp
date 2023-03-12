//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// value_type displace(const_iterator position);

#include <vector>
#include <iterator>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "asan_testing.h"
#include "MoveOnly.h"

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
    int a1[] = {1, 2, 3, 4, 5};
    std::vector<int> l1(a1, a1+5);
    int x = l1.displace(l1.begin());
    assert(x == 1);
    assert(is_contiguous_container_asan_correct(l1));
    assert(l1 == std::vector<int>(a1+1, a1+5));
    }
    {
    int a1[] = {1, 2, 3, 4, 5};
    int e1[] = {1, 3, 4, 5};
    std::vector<int> l1(a1, a1+5);
    int x = l1.displace(l1.begin() + 1);
    assert(x == 2);
    assert(is_contiguous_container_asan_correct(l1));
    assert(l1 == std::vector<int>(e1, e1+4));
    }
    {
    int a1[] = {1, 2, 3};
    std::vector<int> l1(a1, a1+3);
    std::vector<int>::const_iterator i = l1.begin();
    assert(is_contiguous_container_asan_correct(l1));
    ++i;
    int j = l1.displace(i);
    assert(l1.size() == 2);
    assert(std::distance(l1.begin(), l1.end()) == 2);
    assert(j == 2);
    assert(*l1.begin() == 1);
    assert(*std::next(l1.begin()) == 3);
    assert(is_contiguous_container_asan_correct(l1));
    j = l1.displace(l1.begin() + 1);
    assert(j == 3);
    assert(l1.size() == 1);
    assert(std::distance(l1.begin(), l1.end()) == 1);
    assert(*l1.begin() == 1);
    assert(is_contiguous_container_asan_correct(l1));
    j = l1.displace(l1.begin());
    assert(j == 1);
    assert(l1.size() == 0);
    assert(std::distance(l1.begin(), l1.end()) == 0);
    assert(is_contiguous_container_asan_correct(l1));
    }
#if TEST_STD_VER >= 11
    {
    int a1[] = {1, 2, 3};
    std::vector<MoveOnly, min_allocator<MoveOnly>> l1(a1, a1+3);
    std::vector<MoveOnly, min_allocator<MoveOnly>>::const_iterator i = l1.begin();
    assert(is_contiguous_container_asan_correct(l1));
    ++i;
    MoveOnly j = l1.displace(i);
    assert(l1.size() == 2);
    assert(std::distance(l1.begin(), l1.end()) == 2);
    assert(j == 2);
    assert(*l1.begin() == 1);
    assert(*std::next(l1.begin()) == 3);
    assert(is_contiguous_container_asan_correct(l1));
    j = l1.displace(l1.begin() + 1);
    assert(j == 3);
    assert(l1.size() == 1);
    assert(std::distance(l1.begin(), l1.end()) == 1);
    assert(*l1.begin() == 1);
    assert(is_contiguous_container_asan_correct(l1));
    j = l1.displace(l1.begin());
    assert(j == 1);
    assert(l1.size() == 0);
    assert(std::distance(l1.begin(), l1.end()) == 0);
    assert(is_contiguous_container_asan_correct(l1));
    }
#endif

    return true;
}

int main(int, char**)
{
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
