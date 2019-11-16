//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___TYPE_TRAITS_IS_TRIVIALLY_RELOCATABLE_H
#define _LIBCPP___TYPE_TRAITS_IS_TRIVIALLY_RELOCATABLE_H

#include <__config>
#include <__type_traits/integral_constant.h>
#include <__type_traits/is_trivially_destructible.h>
#include <__type_traits/is_trivially_move_constructible.h>
#include <__type_traits/remove_all_extents.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp>
struct _LIBCPP_TEMPLATE_VIS __libcpp_is_trivially_relocatable
  : public integral_constant<bool,
#if __has_extension(trivially_relocatable)
      __is_trivially_relocatable(_Tp)
#else
      is_trivially_move_constructible<typename remove_all_extents<_Tp>::type>::value &&
      is_trivially_destructible<typename remove_all_extents<_Tp>::type>::value
#endif
  > {};

#if _LIBCPP_STD_VER > 14
template <class _Tp>
inline constexpr bool __libcpp_is_trivially_relocatable_v = __libcpp_is_trivially_relocatable<_Tp>::value;
#endif

template <class _Tp>
struct _LIBCPP_TEMPLATE_VIS is_trivially_relocatable
    : public __libcpp_is_trivially_relocatable<_Tp> {};

#if _LIBCPP_STD_VER > 14
template <class _Tp>
inline constexpr bool is_trivially_relocatable_v = is_trivially_relocatable<_Tp>::value;
#endif

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___TYPE_TRAITS_IS_TRIVIALLY_RELOCATABLE_H
