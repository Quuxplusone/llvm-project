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

struct IndirectlyCopyableFromLvaluesOnly {
  struct AssignableFromIntLvalue {
    AssignableFromIntLvalue& operator=(int&);
  };
  AssignableFromIntLvalue& operator*() const;
};
static_assert(!std::indirectly_movable<int*, IndirectlyCopyableFromLvaluesOnly>);

struct IndirectlyCopyableFromIntPtr {
  struct AssignableFromInt {
    AssignableFromInt& operator=(int);
  };
  AssignableFromInt& operator*() const;
};

static_assert( std::indirectly_copyable<int*, int*>);
static_assert( std::indirectly_copyable<const int*, int*>);
static_assert(!std::indirectly_copyable<int*, const int*>);
static_assert(!std::indirectly_copyable<const int*, const int*>);
static_assert(!std::indirectly_copyable<MoveOnly*, MoveOnly*>);
static_assert(!std::indirectly_copyable<const MoveOnly*, MoveOnly*>);
static_assert(!std::indirectly_copyable<int*, IndirectlyMovableFromIntPtr>);
static_assert( std::indirectly_copyable<int*, IndirectlyCopyableFromLvaluesOnly>);
static_assert( std::indirectly_copyable<int*, IndirectlyCopyableFromIntPtr>);
static_assert( std::indirectly_copyable<const int*, IndirectlyCopyableFromIntPtr>);
static_assert(!std::indirectly_copyable<IndirectlyCopyableFromIntPtr, int*>);
static_assert(!std::indirectly_copyable<IndirectlyCopyableFromIntPtr, const int*>);
