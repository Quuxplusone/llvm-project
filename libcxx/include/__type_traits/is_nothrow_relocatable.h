//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___TYPE_TRAITS_IS_NOTHROW_RELOCATABLE_H
#define _LIBCPP___TYPE_TRAITS_IS_NOTHROW_RELOCATABLE_H

#include <__config>
#include <__type_traits/integral_constant.h>
#include <__type_traits/is_constructible.h>
#include <__type_traits/is_destructible.h>
#include <__type_traits/is_nothrow_constructible.h>
#include <__type_traits/is_nothrow_destructible.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp>
struct _LIBCPP_NO_SPECIALIZATIONS is_nothrow_relocatable
  : public integral_constant<bool, is_nothrow_move_constructible<_Tp>::value &&
                                   is_nothrow_destructible<_Tp>::value
  > {};

#if _LIBCPP_STD_VER > 14
template <class _Tp>
_LIBCPP_NO_SPECIALIZATIONS inline constexpr bool is_nothrow_relocatable_v = is_nothrow_relocatable<_Tp>::value;
#endif

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___TYPE_TRAITS_IS_NOTHROW_RELOCATABLE_H
