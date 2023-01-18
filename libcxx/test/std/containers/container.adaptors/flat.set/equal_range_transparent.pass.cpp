//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// template<class K> pair<iterator,iterator>             equal_range(const K& x);
// template<class K> pair<const_iterator,const_iterator> equal_range(const K& x) const;

#include <cassert>
#include <flat_set>
#include <functional>
#include <utility>

#include "test_macros.h"

struct IntMod3 {
    struct Less {
        using is_transparent = void;
        bool operator()(int i, int j) const { return (i % 3) < (j % 3); } 
        bool operator()(int i, const IntMod3& m) const { return (i % 3) < (m.value_ % 3); } 
        bool operator()(const IntMod3& m, int i) const { return (m.value_ % 3) < (i % 3); } 
    };
    explicit IntMod3(int i) : value_(i) {}
    IntMod3(const IntMod3&) = delete;
private:
    int value_;
};

struct Int {
    explicit Int(int i) : value_(i) {}
    Int(const Int&) = delete;
    friend bool operator<(const Int& w, int i) { return w.value_ < i; }
    friend bool operator<(int i, const Int& w) { return i < w.value_; }
private:
    int value_;
};

int main(int, char**)
{
    {
        using M = std::flat_set<int, std::less<>>;
        using R = std::pair<M::iterator, M::iterator>;
        using CR = std::pair<M::const_iterator, M::const_iterator>;
        M m = {1,3};
        ASSERT_SAME_TYPE(decltype(m.equal_range(Int{0})), R);
        ASSERT_SAME_TYPE(decltype(std::as_const(m).equal_range(Int{0})), CR);
        auto begin = m.begin();
        assert(m.equal_range(0) == std::pair(begin, begin));
        assert(m.equal_range(1) == std::pair(begin, begin+1));
        assert(m.equal_range(2) == std::pair(begin+1, begin+1));
        assert(m.equal_range(Int{0}) == std::pair(begin, begin));
        assert(m.equal_range(Int{1}) == std::pair(begin, begin+1));
        assert(m.equal_range(Int{2}) == std::pair(begin+1, begin+1));
        assert(m.equal_range(Int{3}) == std::pair(begin+1, begin+2));
        assert(m.equal_range(Int{4}) == std::pair(begin+2, begin+2));
    }
    {
        using M = std::flat_set<int, IntMod3::Less>;
        using R = std::pair<M::iterator, M::iterator>;
        using CR = std::pair<M::const_iterator, M::const_iterator>;
        M m = {1,3};
        ASSERT_SAME_TYPE(decltype(m.equal_range(Int{0})), R);
        ASSERT_SAME_TYPE(decltype(std::as_const(m).equal_range(Int{0})), CR);
        auto begin = m.begin();
        assert(m.equal_range(6) == std::pair(begin+0, begin+1));
        assert(m.equal_range(7) == std::pair(begin+1, begin+2));
        assert(m.equal_range(8) == std::pair(begin+2, begin+2));
        assert(m.equal_range(IntMod3{0}) == std::pair(begin+0, begin+1));
        assert(m.equal_range(IntMod3{1}) == std::pair(begin+1, begin+2));
        assert(m.equal_range(IntMod3{2}) == std::pair(begin+2, begin+2));
        assert(m.equal_range(IntMod3{3}) == std::pair(begin+0, begin+1));
        assert(m.equal_range(IntMod3{4}) == std::pair(begin+1, begin+2));
        assert(m.equal_range(IntMod3{5}) == std::pair(begin+2, begin+2));
    }
    return 0;
}
