//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___ALGORITHM_LEXICOGRAPHICAL_COMPARE_H
#define _LIBCPP___ALGORITHM_LEXICOGRAPHICAL_COMPARE_H

#include <__algorithm/comp.h>
#include <__algorithm/unwrap_iter.h>
#include <__config>
#include <__iterator/iterator_traits.h>
#include <__string/constexpr_c_functions.h>
#include <__type_traits/desugars_to.h>
#include <__type_traits/enable_if.h>
#include <__type_traits/integral_constant.h>
#include <__type_traits/is_constant_evaluated.h>
#include <__type_traits/is_trivially_lexicographically_comparable.h>
#include <__type_traits/is_volatile.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _InputIterator1, class _InputIterator2, class _BinaryPredicate>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 bool __lexicographical_compare_iter_impl(
    _InputIterator1 __first1,
    _InputIterator1 __last1,
    _InputIterator2 __first2,
    _InputIterator2 __last2,
    _BinaryPredicate& __comp) {
  for (; __first2 != __last2; ++__first1, (void)++__first2) {
    if (__first1 == __last1 || __comp(*__first1, *__first2))
      return true;
    if (__comp(*__first2, *__first1))
      return false;
  }
  return false;
}

template <class _Tp,
          class _Up,
          class _BinaryPredicate,
          __enable_if_t<__desugars_to_v<__less_tag, _BinaryPredicate, _Tp, _Up> && !is_volatile<_Tp>::value &&
                        !is_volatile<_Up>::value && __libcpp_is_trivially_lexicographically_comparable<_Tp, _Up>::value,
                        int> = 0>
_LIBCPP_NODISCARD inline _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 bool
__lexicographical_compare_iter_impl(_Tp* __first1, _Tp* __last1, _Up* __first2, _Up* __last2, _BinaryPredicate&) {
  auto __n1 = __last1 - __first1;
  auto __n2 = __last2 - __first2;
  if (__n1 < __n2) {
    return std::__constexpr_memcmp(__first1, __first2, __element_count(__n1)) <= 0;
  } else {
    return std::__constexpr_memcmp(__first1, __first2, __element_count(__n2)) < 0;
  }
}

template <class _InputIterator1, class _InputIterator2, class _Compare>
_LIBCPP_NODISCARD inline _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 bool lexicographical_compare(
    _InputIterator1 __first1,
    _InputIterator1 __last1,
    _InputIterator2 __first2,
    _InputIterator2 __last2,
    _Compare __comp) {
  return std::__lexicographical_compare_iter_impl(
    std::__unwrap_iter(__first1), std::__unwrap_iter(__last1),
    std::__unwrap_iter(__first2), std::__unwrap_iter(__last2), __comp);
}

template <class _InputIterator1, class _InputIterator2>
_LIBCPP_NODISCARD inline _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 bool lexicographical_compare(
    _InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2, _InputIterator2 __last2) {
  return std::lexicographical_compare(__first1, __last1, __first2, __last2, __less<>());
}

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___ALGORITHM_LEXICOGRAPHICAL_COMPARE_H
