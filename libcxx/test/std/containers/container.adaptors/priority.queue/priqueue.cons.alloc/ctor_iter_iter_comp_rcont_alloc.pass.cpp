//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <queue>

// template <class InputIterator>
//   priority_queue(InputIterator first, InputIterator last,
//                  const Compare& comp, Container&& c, const Alloc& a);

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "MoveOnly.h"

template<class T, class Cont, class Comp = std::less<T> >
struct PQ : std::priority_queue<T, Cont, Comp> {
    using std::priority_queue<T, Cont, Comp>::priority_queue;
    using std::priority_queue<T, Cont, Comp>::c;
};

int main(int, char**)
{
    using Alloc = test_allocator<MoveOnly>;
    int a[] = {3, 5, 2, 0, 6, 8, 1};
    const int n = sizeof(a)/sizeof(a[0]);
    PQ<MoveOnly, std::vector<MoveOnly, Alloc>>
        q(a+n/2, a+n, std::less<MoveOnly>(),
          std::vector<MoveOnly, Alloc>(a, a+n/2), Alloc(2));
    assert(q.size() == n);
    assert(q.top() == MoveOnly(8));
    assert(q.c.get_allocator() == Alloc(2));

    return 0;
}
