//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// template<class K> size_type count(const K& x) const;

#include <cassert>
#include <flat_set>
#include <functional>

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
        M m = {1,3};
        ASSERT_SAME_TYPE(decltype(m.count(Int{0})), size_t);
        assert(m.count(0) == 0);
        assert(m.count(1) == 1);
        assert(m.count(2) == 0);
        assert(m.count(Int{0}) == 0);
        assert(m.count(Int{1}) == 1);
        assert(m.count(Int{2}) == 0);
        assert(m.count(Int{3}) == 1);
        assert(m.count(Int{4}) == 0);
    }
    {
        using M = std::flat_set<int, IntMod3::Less>;
        M m = {1,3};
        ASSERT_SAME_TYPE(decltype(m.count(IntMod3{0})), size_t);
        assert(m.count(6) == 1);
        assert(m.count(7) == 1);
        assert(m.count(8) == 0);
        assert(m.count(IntMod3{0}) == 1);
        assert(m.count(IntMod3{1}) == 1);
        assert(m.count(IntMod3{2}) == 0);
        assert(m.count(IntMod3{3}) == 1);
        assert(m.count(IntMod3{4}) == 1);
        assert(m.count(IntMod3{5}) == 0);
    }
    return 0;
}
