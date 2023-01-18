//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// template <class... Args>
//   iterator emplace_hint(const_iterator position, Args&&... args);

#include <cassert>
#include <concepts>
#include <deque>
#include <flat_set>
#include <functional>

#include "test_macros.h"
#include "../../Emplaceable.h"
#include "DefaultOnly.h"
#include "min_allocator.h"

int main(int, char**)
{
#if 0
    {
        typedef std::flat_set<DefaultOnly> M;
        typedef M::iterator R;
        M m;
        assert(DefaultOnly::count == 0);
        std::same_as<R> auto r = m.emplace_hint(m.cend());
        assert(r == m.begin());
        assert(m.size() == 1);
        assert(*m.begin() == DefaultOnly());
        assert(DefaultOnly::count == 1);

        r = m.emplace_hint(m.cbegin());
        assert(r == m.begin());
        assert(m.size() == 1);
        assert(*m.begin() == DefaultOnly());
        assert(DefaultOnly::count == 1);
    }
    assert(DefaultOnly::count == 0);
#endif
    {
        typedef std::flat_set<Emplaceable> M;
        typedef M::iterator R;
        M m;
        std::same_as<R> auto r = m.emplace_hint(m.cend());
        assert(r == m.begin());
        assert(m.size() == 1);
        assert(*m.begin() == Emplaceable());
        r = m.emplace_hint(m.cend(), 2, 3.5);
        assert(r == std::next(m.begin()));
        assert(m.size() == 2);
        assert(*r == Emplaceable(2, 3.5));
        r = m.emplace_hint(m.cbegin(), 2, 3.5);
        assert(r == std::next(m.begin()));
        assert(m.size() == 2);
        assert(*r == Emplaceable(2, 3.5));
    }
    {
        typedef std::flat_set<int> M;
        typedef M::iterator R;
        M m;
        std::same_as<R> auto r = m.emplace_hint(m.cend(), 2);
        assert(r == m.begin());
        assert(m.size() == 1);
        assert(*r == 2);
    }
    {
        typedef std::flat_set<int, std::greater<int>, std::deque<int, min_allocator<int>>> M;
        typedef M::iterator R;
        M m;
        std::same_as<R> auto r = m.emplace_hint(m.cend(), 2);
        assert(r == m.begin());
        assert(m.size() == 1);
        assert(*r == 2);
    }
    return 0;
}
