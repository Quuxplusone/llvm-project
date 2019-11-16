//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: libcpp-no-concepts

// template<class T>
// concept relocatable;

#include <concepts>

#include <cassert>

static_assert(std::relocatable<int>);
static_assert(std::relocatable<const int>);
static_assert(std::relocatable<int*>);
static_assert(std::relocatable<int(*)()>);

// Array types are not move-constructible in the concepts sense, thus not "relocatable".
static_assert(!std::relocatable<int[]>);
static_assert(!std::relocatable<const int[]>);
static_assert(!std::relocatable<int[4]>);
static_assert(!std::relocatable<const int[4]>);

static_assert(!std::relocatable<void>);
static_assert(!std::relocatable<const void>);
static_assert(!std::relocatable<int()>);

struct NotDestructible {
    NotDestructible(const NotDestructible&);
    NotDestructible(NotDestructible&&);
    ~NotDestructible() = delete;
};

static_assert(!std::relocatable<NotDestructible>);

struct NotMoveConstructible {
    NotMoveConstructible(const NotMoveConstructible&);
    NotMoveConstructible(NotMoveConstructible&&) = delete;
};

static_assert(!std::relocatable<NotMoveConstructible>);

struct NotCopyConstructible {
    NotCopyConstructible(const NotCopyConstructible&) = delete;
    NotCopyConstructible(NotCopyConstructible&&);
};

static_assert(std::relocatable<NotCopyConstructible>);  // OK

// Reference types ARE move-constructible in the concepts sense, therefore "relocatable".
static_assert(std::relocatable<int&>);
static_assert(std::relocatable<int&&>);
static_assert(std::relocatable<int(&)()>);
static_assert(std::relocatable<NotMoveConstructible&>);
static_assert(std::relocatable<NotCopyConstructible&>);
static_assert(std::relocatable<NotDestructible&>);

bool moveconstructible_subsumes_relocatable(std::move_constructible auto) requires true { return true; }
void moveconstructible_subsumes_relocatable(std::relocatable auto) { }

bool relocatable_subsumes_moveconstructible(std::relocatable auto) requires true { return true; }
void relocatable_subsumes_moveconstructible(std::move_constructible auto) { }

int main(int, char**)
{
    assert(moveconstructible_subsumes_relocatable(42));
    assert(relocatable_subsumes_moveconstructible(42));

    return 0;
}
