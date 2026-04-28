//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___ALGORITHM_IS_HEAP_UNTIL_H
#define _LIBCPP___ALGORITHM_IS_HEAP_UNTIL_H

#include <__algorithm/comp.h>
#include <__algorithm/comp_ref_type.h>
#include <__config>
#include <__iterator/iterator_traits.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Compare, class _ForwardIterator, class _Sentinel>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 _ForwardIterator
__is_heap_until(_ForwardIterator __first, _Sentinel __last, _Compare&& __comp) {
  _ForwardIterator __child = __first;
  if (__child == __last) {
    return __child;
  }
  while (true) {
    ++__child;
    if (__child == __last || __comp(*__first, *__child)) {
      break;
    }
    ++__child;
    if (__child == __last || __comp(*__first, *__child)) {
      break;
    }
    ++__first;
  }
  return __child;
}

template <class _ForwardIterator, class _Compare>
[[__nodiscard__]] inline _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 _ForwardIterator
is_heap_until(_ForwardIterator __first, _ForwardIterator __last, _Compare __comp) {
  return std::__is_heap_until(__first, __last, static_cast<__comp_ref_type<_Compare> >(__comp));
}

template <class _ForwardIterator>
[[__nodiscard__]] inline _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 _ForwardIterator
is_heap_until(_ForwardIterator __first, _ForwardIterator __last) {
  return std::__is_heap_until(__first, __last, __less<>());
}

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___ALGORITHM_IS_HEAP_UNTIL_H
