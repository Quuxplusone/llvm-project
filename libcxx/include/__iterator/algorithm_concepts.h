// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___ITERATOR_ALGORITHM_CONCEPTS_H
#define _LIBCPP___ITERATOR_ALGORITHM_CONCEPTS_H

#include <__config>
#include <__concepts/copyable.h>
#include <__concepts/movable.h>
#include <__functional/identity.h>
#include <__functional/ranges_operations.h>
#include <__iterator/concepts.h>
#include <__iterator/indirectly_copyable.h>
#include <__iterator/indirectly_movable.h>
#include <__iterator/indirectly_swappable.h>
#include <__iterator/iterator_traits.h>
#include <__iterator/projected.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

#if _LIBCPP_STD_VER > 17 && !defined(_LIBCPP_HAS_NO_CONCEPTS)

// [alg.req.ind.cmp]

template<class _It1, class _It2, class _Pred, class _Proj1 = identity, class _Proj2 = identity>
concept indirectly_comparable =
  indirect_binary_predicate<_Pred, projected<_It1, _Proj1>, projected<_It2, _Proj2>>;

// [alg.req.permutable]

template<class _In>
concept permutable =
  forward_iterator<_In> &&
  indirectly_movable_storable<_In, _In> &&
  indirectly_swappable<_In, _In>;

// [alg.req.mergeable]

#if !defined(_LIBCPP_HAS_NO_RANGES)

template<class _In1, class _In2, class _Out, class _Pred = ranges::less,
         class _Proj1 = identity, class _Proj2 = identity>
concept mergeable =
  input_iterator<_In1> &&
  input_iterator<_In2> &&
  weakly_incrementable<_Out> &&
  indirectly_copyable<_In1, _Out> &&
  indirectly_copyable<_In2, _Out> &&
  indirect_strict_weak_order<_Pred, projected<_In1, _Proj1>, projected<_In2, _Proj2>>;

// [alg.req.sortable]

template<class _It, class _Pred = ranges::less, class _Proj = identity>
concept sortable =
  permutable<_It> &&
  indirect_strict_weak_order<_Pred, projected<_It, _Proj>>;

#endif // !defined(_LIBCPP_HAS_NO_RANGES)

#endif // _LIBCPP_STD_VER > 17 && !defined(_LIBCPP_HAS_NO_CONCEPTS)

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___ITERATOR_ALGORITHM_CONCEPTS_H
