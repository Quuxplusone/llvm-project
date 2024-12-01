//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___UTILITY_INTEGER_SEQUENCE_BINDINGS_H
#define _LIBCPP___UTILITY_INTEGER_SEQUENCE_BINDINGS_H

#include <__config>
#include <__tuple/tuple_element.h>
#include <__tuple/tuple_size.h>
#include <__type_traits/integral_constant.h>
#include <__utility/integer_sequence.h>
#include <__cstddef/size_t.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

#if _LIBCPP_STD_VER >= 17

template<class _Tp, _Tp... _Ip>
struct tuple_size<integer_sequence<_Tp, _Ip...>> : integral_constant<size_t, sizeof...(_Ip)> {};

template <size_t _Index, class _Tp, _Tp... _Ip>
struct tuple_element<_Index, integer_sequence<_Tp, _Ip...>> {
  using type = _Tp;
};

template <size_t _Index, class _Tp, _Tp... _Ip>
_LIBCPP_HIDE_FROM_ABI constexpr _Tp get(integer_sequence<_Tp, _Ip...>) {
  constexpr _Tp (*__actions[])() = {
    []() -> _Tp { return _Ip; }...
  };
  return __actions[_Index]();
}

#endif // _LIBCPP_STD_VER >= 17

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___UTILITY_INTEGER_SEQUENCE_H
