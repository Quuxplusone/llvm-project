// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___MEMORY_UNINITIALIZED_RELOCATE_BACKWARD_H
#define _LIBCPP___MEMORY_UNINITIALIZED_RELOCATE_BACKWARD_H

#include <__config>
#include <__iterator/iterator_traits.h>
#include <__memory/addressof.h>
#include <__memory/construct_at.h>
#include <__memory/pointer_traits.h>
#include <__memory/relocate_at.h>
#include <__type_traits/integral_constant.h>
#include <__type_traits/is_nothrow_constructible.h>
#include <__type_traits/is_trivially_relocatable.h>
#include <__type_traits/is_volatile.h>
#include <__type_traits/remove_cvref.h>
#include <__utility/move.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _BidirIt1, class _BidirIt2>
_LIBCPP_HIDE_FROM_ABI
_BidirIt2
__uninitialized_relocate_backward_impl(_BidirIt1 __first, _BidirIt1 __last,
                                       _BidirIt2 __result, false_type, false_type)
{
  _BidirIt2 __orig_result = __result;
  while (__first != __last) {
    --__last;
    --__result;
#ifndef _LIBCPP_HAS_NO_EXCEPTIONS
    try {
#endif
      std::__libcpp_relocate_at2(0, std::addressof(*__last), std::addressof(*__result));
#ifndef _LIBCPP_HAS_NO_EXCEPTIONS
    } catch (...) {
      std::__destroy(++__result, __orig_result);
      std::__destroy(__first, __last);
      throw;
    }
#endif
  }
  return __result;
}

template <class _BidirIt1, class _BidirIt2>
_LIBCPP_HIDE_FROM_ABI
_BidirIt2
__uninitialized_relocate_backward_impl(_BidirIt1 __first, _BidirIt1 __last,
                                       _BidirIt2 __result, false_type, true_type)
{
  while (__first != __last) {
    --__last;
    --__result;
    std::__libcpp_relocate_at2(0, std::addressof(*__last), std::addressof(*__result));
  }
  return __result;
}

template <class _BidirIt1, class _BidirIt2, class _LoopWithoutCatchIgnored>
_LIBCPP_HIDE_FROM_ABI
_BidirIt2
__uninitialized_relocate_backward_impl(_BidirIt1 __first, _BidirIt1 __last,
                                       _BidirIt2 __result, true_type, _LoopWithoutCatchIgnored)
{
  auto __count = __last - __first;
  if (__count != 0) {
    char *__firstbyte = (char *)std::__to_address(__first);
    auto __nbytes = (char *)std::__to_address(__last) - __firstbyte;
    __result -= __count;
    ::__builtin_memmove(std::__to_address(__result), __firstbyte, __nbytes);
  }
  return __result;
}

template <class _BidirIt1, class _BidirIt2>
_LIBCPP_HIDE_FROM_ABI
_BidirIt2
uninitialized_relocate_backward(_BidirIt1 __first, _BidirIt1 __last,
                                _BidirIt2 __result)
{
  using _St = decltype(std::move(*__first));
  using _Dt = typename iterator_traits<_BidirIt2>::value_type;
  using _ElementTypeIsMemcpyable = integral_constant<bool,
    __is_same_uncvref<_St, _Dt>::value &&
    __libcpp_is_trivially_relocatable<_Dt>::value &&
    !is_volatile<_St>::value && !is_volatile<_Dt>::value
  >;
  using _SingleMemcpy = integral_constant<bool,
    _ElementTypeIsMemcpyable::value &&
    __is_cpp17_contiguous_iterator<_BidirIt1>::value &&
    __is_cpp17_contiguous_iterator<_BidirIt2>::value
  >;
  using _LoopWithoutCatch = is_nothrow_constructible<_Dt, _St>;
  return std::__uninitialized_relocate_backward_impl(__first, __last, __result,
    _SingleMemcpy(), _LoopWithoutCatch());
}

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___MEMORY_UNINITIALIZED_RELOCATE_BACKWARD_H
