// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___ITERATOR_INDIRECTLY_COPYABLE_H
#define _LIBCPP___ITERATOR_INDIRECTLY_COPYABLE_H

#include <__config>
#include <__concepts/assignable.h>
#include <__concepts/constructible.h>
#include <__concepts/copyable.h>
#include <__iterator/concepts.h>
#include <__iterator/iterator_traits.h>
#include <__iterator/readable_traits.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

#if _LIBCPP_STD_VER > 17 && !defined(_LIBCPP_HAS_NO_CONCEPTS)

// [alg.req.ind.copy]

template<class _In, class _Out>
concept indirectly_copyable =
  indirectly_readable<_In> &&
  indirectly_writable<_Out, iter_reference_t<_In>>;

template<class _In, class _Out>
concept indirectly_copyable_storable =
  indirectly_copyable<_In, _Out> &&
  indirectly_writable<_Out, iter_value_t<_In>&> &&
  indirectly_writable<_Out, const iter_value_t<_In>&> &&
  indirectly_writable<_Out, iter_value_t<_In>&&> &&
  indirectly_writable<_Out, const iter_value_t<_In>&&> &&
  copyable<iter_value_t<_In>> &&
  constructible_from<iter_value_t<_In>, iter_reference_t<_In>> &&
  assignable_from<iter_value_t<_In>&, iter_reference_t<_In>>;

#endif // _LIBCPP_STD_VER > 17 && !defined(_LIBCPP_HAS_NO_CONCEPTS)

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___ITERATOR_INDIRECTLY_COPYABLE_H
