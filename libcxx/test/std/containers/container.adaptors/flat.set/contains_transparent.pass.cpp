//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// template<class K> bool contains(const K& x) const;

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
        M m = {1,3};
        assert(!m.contains(0));
        assert( m.contains(1));
        assert(!m.contains(2));
        assert(!m.contains(Int{0}));
        assert( m.contains(Int{1}));
        assert(!m.contains(Int{2}));
        assert( std::as_const(m).contains(Int{3}));
        assert(!std::as_const(m).contains(Int{4}));
    }
    {
        using M = std::flat_set<int, IntMod3::Less>;
        M m = {1,3};
        assert( m.contains(6));
        assert( m.contains(7));
        assert(!m.contains(8));
        assert( m.contains(IntMod3{0}));
        assert( m.contains(IntMod3{1}));
        assert(!m.contains(IntMod3{2}));
        assert( m.contains(IntMod3{3}));
        assert( std::as_const(m).contains(IntMod3{4}));
        assert(!std::as_const(m).contains(IntMod3{5}));
    }
    return 0;
}
