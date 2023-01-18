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
//   pair<iterator, bool> emplace(Args&&... args);

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
        typedef std::pair<M::iterator, bool> R;
        M m;
        assert(DefaultOnly::count == 0);
        std::same_as<R> auto r = m.emplace();
        assert(r.second);
        assert(r.first == m.begin());
        assert(m.size() == 1);
        assert(*m.begin() == DefaultOnly());
        assert(DefaultOnly::count == 1);

        r = m.emplace();
        assert(!r.second);
        assert(r.first == m.begin());
        assert(m.size() == 1);
        assert(*m.begin() == DefaultOnly());
        assert(DefaultOnly::count == 1);
    }
    assert(DefaultOnly::count == 0);
#endif
    {
        typedef std::flat_set<Emplaceable> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        std::same_as<R> auto r = m.emplace();
        assert(r.second);
        assert(r.first == m.begin());
        assert(m.size() == 1);
        assert(*m.begin() == Emplaceable());
        r = m.emplace(2, 3.5);
        assert(r.second);
        assert(r.first == std::next(m.begin()));
        assert(m.size() == 2);
        assert(*r.first == Emplaceable(2, 3.5));
        r = m.emplace(2, 3.5);
        assert(!r.second);
        assert(r.first == std::next(m.begin()));
        assert(m.size() == 2);
        assert(*r.first == Emplaceable(2, 3.5));
    }
    {
        typedef std::flat_set<int> M;
        typedef std::pair<M::iterator, bool> R;
        M m = {1, 3, 4};
        std::same_as<R> auto r = m.emplace(2);
        assert(r == std::pair(m.begin() + 1, true));
        assert(*r.first == 2);
        assert((m == M{1, 2, 3, 4}));
        r = m.emplace(5);
        assert(r == std::pair(m.begin() + 4, true));
        assert(*r.first == 5);
        assert((m == M{1, 2, 3, 4, 5}));
        r = m.emplace(2);
        assert(r == std::pair(m.begin() + 1, false));
        assert(*r.first == 2);
        assert((m == M{1, 2, 3, 4, 5}));
        r = m.emplace(); // value-initialize int(0)
        assert(r == std::pair(m.begin(), true));
        assert(*r.first == 0);
        assert((m == M{0, 1, 2, 3, 4, 5}));
    }
    {
        typedef std::flat_set<int, std::greater<int>, std::deque<int, min_allocator<int>>> M;
        typedef std::pair<M::iterator, bool> R;
        M m = {1, 3, 4};
        std::same_as<R> auto r = m.emplace(2);
        assert(r == std::pair(m.begin() + 2, true));
        assert(*r.first == 2);
        assert((m == M{1, 2, 3, 4}));
        r = m.emplace(5);
        assert(r == std::pair(m.begin(), true));
        assert(*r.first == 5);
        assert((m == M{1, 2, 3, 4, 5}));
        r = m.emplace(2);
        assert(r == std::pair(m.begin() + 3, false));
        assert(*r.first == 2);
        assert((m == M{1, 2, 3, 4, 5}));
        r = m.emplace(); // value-initialize int(0)
        assert(r == std::pair(m.begin() + 5, true));
        assert(*r.first == 0);
        assert((m == M{0, 1, 2, 3, 4, 5}));
    }
    return 0;
}
