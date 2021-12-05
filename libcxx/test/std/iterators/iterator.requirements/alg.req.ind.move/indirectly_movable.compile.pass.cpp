//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: libcpp-no-concepts

// template<class In, class Out>
// concept indirectly_movable;

#include <iterator>

#include "test_macros.h"
#include "MoveOnly.h"

struct IndirectlyMovableFromIntPtr {
  struct AssignableFromIntRvalue {
    AssignableFromIntRvalue& operator=(int&&);
  };
  AssignableFromIntRvalue& operator*() const;
};

static_assert( std::indirectly_movable<int*, int*>);
static_assert( std::indirectly_movable<const int*, int*>);
static_assert(!std::indirectly_movable<int*, const int*>);
static_assert(!std::indirectly_movable<const int*, const int*>);
static_assert( std::indirectly_movable<MoveOnly*, MoveOnly*>);
static_assert(!std::indirectly_movable<const MoveOnly*, MoveOnly*>);
static_assert(!std::indirectly_movable<MoveOnly*, const MoveOnly*>);
static_assert(!std::indirectly_movable<const MoveOnly*, const MoveOnly*>);
static_assert( std::indirectly_movable<int*, IndirectlyMovableFromIntPtr>);
static_assert(!std::indirectly_movable<const int*, IndirectlyMovableFromIntPtr>);
static_assert(!std::indirectly_movable<IndirectlyMovableFromIntPtr, int*>);
static_assert(!std::indirectly_movable<IndirectlyMovableFromIntPtr, const int*>);
