//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <queue>

// template <class InputIterator>
//   priority_queue(InputIterator first, InputIterator last,
//                  const Compare& comp, const Container& c, const Alloc& a);

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"

template<class T, class Cont, class Comp = std::less<T> >
struct PQ : std::priority_queue<T, Cont, Comp> {
    using std::priority_queue<T, Cont, Comp>::priority_queue;
    using std::priority_queue<T, Cont, Comp>::c;
};

int main(int, char**)
{
    typedef test_allocator<int> Alloc;
    int a[] = {3, 5, 2, 0, 6, 8, 1};
    const int n = sizeof(a)/sizeof(a[0]);
    std::vector<int, Alloc> v(a, a+n/2);
    PQ<int, std::vector<int, Alloc> >
        q(a+n/2, a+n, std::less<int>(), v, Alloc(2));
    assert(q.size() == n);
    assert(q.top() == 8);
    assert(q.c.get_allocator() == Alloc(2));

    return 0;
}
