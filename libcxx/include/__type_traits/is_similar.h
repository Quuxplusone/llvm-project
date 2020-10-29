//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___TYPE_TRAITS_IS_SIMILAR_H
#define _LIBCPP___TYPE_TRAITS_IS_SIMILAR_H

#include <__config>
#include <__type_traits/conditional.h>
#include <__type_traits/integral_constant.h>
#include <__type_traits/is_const.h>
#include <__type_traits/is_same.h>
#include <__type_traits/is_volatile.h>
#include <__type_traits/remove_cv.h>
#include <cstddef>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp, class _Up>
struct is_similar : conditional<
        is_const<_Tp>::value || is_volatile<_Tp>::value ||
            is_const<_Up>::value || is_volatile<_Up>::value,
        is_similar<typename remove_cv<_Tp>::type, typename remove_cv<_Up>::type>,
        is_same<_Tp, _Up>
    >::type {};

template <class _Tp, class _Up>
struct is_similar<_Tp*, _Up*>: is_similar<_Tp, _Up> {};

template <class _Tp, class _Up, class _Cp>
struct is_similar<_Tp _Cp::*, _Up _Cp::*>: is_similar<_Tp, _Up> {};

template <class _Tp, class _Up>
struct is_similar<_Tp[], _Up[]>: is_similar<_Tp, _Up> {};

template <class _Tp, class _Up, size_t _Np>
struct is_similar<_Tp[], _Up[_Np]>: is_similar<_Tp, _Up> {};

template <class _Tp, class _Up, size_t _Np>
struct is_similar<_Tp[_Np], _Up[]>: is_similar<_Tp, _Up> {};

template <class _Tp, class _Up, size_t _Np>
struct is_similar<_Tp[_Np], _Up[_Np]>: is_similar<_Tp, _Up> {};

#if _LIBCPP_STD_VER > 14
template <class _Tp, class _Up>
inline constexpr bool is_similar_v = is_similar<_Tp, _Up>::value;
#endif

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___TYPE_TRAITS_IS_SIMILAR_H
