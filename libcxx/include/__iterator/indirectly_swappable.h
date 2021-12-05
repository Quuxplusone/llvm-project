// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___ITERATOR_INDIRECTLY_SWAPPABLE_H
#define _LIBCPP___ITERATOR_INDIRECTLY_SWAPPABLE_H

#include <__config>
#include <__iterator/concepts.h>
#include <__iterator/iter_swap.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

#if _LIBCPP_STD_VER > 17 && !defined(_LIBCPP_HAS_NO_CONCEPTS) && !defined(_LIBCPP_HAS_NO_RANGES)

// [alg.req.ind.swap]

template<class _It1, class _It2 = _It1>
concept indirectly_swappable =
  indirectly_readable<_It1> && indirectly_readable<_It2> &&
  requires (const _It1 __it1, const _It2 __it2) {
    ranges::iter_swap(__it1, __it1);
    ranges::iter_swap(__it2, __it2);
    ranges::iter_swap(__it1, __it2);
    ranges::iter_swap(__it2, __it1);
  };

#endif // _LIBCPP_STD_VER > 17 && !defined(_LIBCPP_HAS_NO_CONCEPTS) && !defined(_LIBCPP_HAS_NO_RANGES)

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___ITERATOR_INDIRECTLY_SWAPPABLE_H
