//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___ALGORITHM_COPY_MOVE_COMMON_H
#define _LIBCPP___ALGORITHM_COPY_MOVE_COMMON_H

#include <__algorithm/iterator_operations.h>
#include <__algorithm/unwrap_iter.h>
#include <__algorithm/unwrap_range.h>
#include <__config>
#include <__iterator/iterator_traits.h>
#include <__memory/pointer_traits.h>
#include <__string/constexpr_c_functions.h>
#include <__type_traits/enable_if.h>
#include <__type_traits/is_always_bitcastable.h>
#include <__type_traits/is_constant_evaluated.h>
#include <__type_traits/is_copy_constructible.h>
#include <__type_traits/is_trivially_assignable.h>
#include <__type_traits/is_trivially_constructible.h>
#include <__type_traits/is_trivially_copyable.h>
#include <__type_traits/is_trivially_destructible.h>
#include <__type_traits/is_volatile.h>
#include <__utility/move.h>
#include <__utility/pair.h>
#include <__utility/swap.h>
#include <cstddef>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

// Type traits.

template <class _From, class _To>
struct __can_lower_copy_assignment_to_memmove {
  static const bool value =
    // If the types are always bitcastable, it's valid to do a bitwise copy between them.
    __is_always_bitcastable<_From, _To>::value &&
    // Reject conversions that wouldn't be performed by the regular built-in assignment (e.g. between arrays).
    is_trivially_assignable<_To&, const _From&>::value &&
    // `memmove` doesn't accept `volatile` pointers, make sure the optimization SFINAEs away in that case.
    !is_volatile<_From>::value &&
    !is_volatile<_To>::value;
};

template <class _From, class _To>
struct __can_lower_move_assignment_to_memmove {
  static const bool value =
    __is_always_bitcastable<_From, _To>::value &&
    is_trivially_assignable<_To&, _From&&>::value &&
    !is_volatile<_From>::value &&
    !is_volatile<_To>::value;
};

template <class _From, class _To>
struct __can_lower_swap_to_memswap {
  static const bool value =
    __is_always_bitcastable<_From, _To>::value &&
    __is_always_bitcastable<_To, _From>::value &&
    // These are the operations performed by `From& = std::exchange(To&, From&&)`.
    is_trivially_constructible<_To, _To&&>::value &&
    is_trivially_assignable<_To&, _From&&>::value &&
    is_trivially_assignable<_From&, _To&&>::value &&
    is_trivially_destructible<_To>::value &&
    !is_volatile<_From>::value &&
    !is_volatile<_To>::value;
};

// `memmove` algorithms implementation.

template <class _In, class _Out>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX14 pair<_In*, _Out*>
__copy_trivial_impl(_In* __first, _In* __last, _Out* __result) {
  const size_t __n = static_cast<size_t>(__last - __first);

  std::__constexpr_memmove(__result, __first, __element_count(__n));

  return std::make_pair(__last, __result + __n);
}

template <class _In, class _Out>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX14 pair<_In*, _Out*>
__copy_backward_trivial_impl(_In* __first, _In* __last, _Out* __result) {
  const size_t __n = static_cast<size_t>(__last - __first);
  __result -= __n;

  std::__constexpr_memmove(__result, __first, __element_count(__n));

  return std::make_pair(__last, __result);
}

template <class _Tp, class _Up>
_LIBCPP_HIDE_FROM_ABI pair<_Tp*, _Up*>
__swap_ranges_trivial_impl(_Tp* __first1, _Tp* __last1, _Up* __first2) {
  const size_t __n = static_cast<size_t>(__last1 - __first1);
  std::__libcpp_memswap(__first1, __first2, __n * sizeof(_Tp));
  return std::make_pair(__last1, __first2 + __n);
}

// Iterator unwrapping and dispatching to the correct overload.

template <class _F1, class _F2>
struct __overload : _F1, _F2 {
  using _F1::operator();
  using _F2::operator();
};

template <class _InIter, class _Sent, class _OutIter, class = void>
struct __can_rewrap : false_type {};

template <class _InIter, class _Sent, class _OutIter>
struct __can_rewrap<_InIter,
                    _Sent,
                    _OutIter,
                    // Note that sentinels are always copy-constructible.
                    __enable_if_t< is_copy_constructible<_InIter>::value &&
                                   is_copy_constructible<_OutIter>::value > > : true_type {};

template <class _Algorithm,
          class _InIter,
          class _Sent,
          class _OutIter,
          __enable_if_t<__can_rewrap<_InIter, _Sent, _OutIter>::value, int> = 0>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX17 pair<_InIter, _OutIter>
__unwrap_and_dispatch(_InIter __first, _Sent __last, _OutIter __out_first) {
  auto __range  = std::__unwrap_range(__first, std::move(__last));
  auto __result = _Algorithm()(std::move(__range.first), std::move(__range.second), std::__unwrap_iter(__out_first));
  return std::make_pair(std::__rewrap_range<_Sent>(std::move(__first), std::move(__result.first)),
                                 std::__rewrap_iter(std::move(__out_first), std::move(__result.second)));
}

template <class _Algorithm,
          class _InIter,
          class _Sent,
          class _OutIter,
          __enable_if_t<!__can_rewrap<_InIter, _Sent, _OutIter>::value, int> = 0>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX17 pair<_InIter, _OutIter>
__unwrap_and_dispatch(_InIter __first, _Sent __last, _OutIter __out_first) {
  return _Algorithm()(std::move(__first), std::move(__last), std::move(__out_first));
}

template <class _AlgPolicy,
          class _NaiveAlgorithm,
          class _OptimizedAlgorithm,
          class _InIter,
          class _Sent,
          class _OutIter>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX17 pair<_InIter, _OutIter>
__dispatch_copy_or_move(_InIter __first, _Sent __last, _OutIter __out_first) {
  using _Algorithm = __overload<_NaiveAlgorithm, _OptimizedAlgorithm>;
  return std::__unwrap_and_dispatch<_Algorithm>(std::move(__first), std::move(__last), std::move(__out_first));
}

template <class _Algorithm,
          class _Iter1,
          class _Sent1,
          class _Iter2,
          class _Sent2,
          __enable_if_t<__can_rewrap<_Iter1, _Sent1, _Iter2>::value, int> = 0>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX17 pair<_Iter1, _Iter2>
__unwrap_and_dispatch(_Iter1 __first1, _Sent1 __last1, _Iter2 __first2, _Sent2 __last2) {
  auto __range1 = std::__unwrap_range(__first1, std::move(__last1));
  auto __range2 = std::__unwrap_range(__first2, std::move(__last2));
  auto __result = _Algorithm()(std::move(__range1.first), std::move(__range1.second), std::move(__range2.first), std::move(__range2.second));
  return std::make_pair(std::__rewrap_range<_Sent1>(std::move(__first1), std::move(__result.first)),
                        std::__rewrap_range<_Sent2>(std::move(__first2), std::move(__result.second)));
}

template <class _Algorithm,
          class _Iter1,
          class _Sent1,
          class _Iter2,
          class _Sent2,
          __enable_if_t<!__can_rewrap<_Iter1, _Sent1, _Iter2>::value, int> = 0>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX17 pair<_Iter1, _Iter2>
__unwrap_and_dispatch(_Iter1 __first1, _Sent1 __last1, _Iter2 __first2, _Sent2 __last2) {
  return _Algorithm()(std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2));
}

template <class _AlgPolicy,
          class _NaiveAlgorithm,
          class _OptimizedAlgorithm,
          class _Iter1,
          class _Sent1,
          class _Iter2,
          class _Sent2>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX17 pair<_Iter1, _Iter2>
__dispatch_copy_or_move(_Iter1 __first1, _Sent1 __last1, _Iter2 __first2, _Sent2 __last2) {
  using _Algorithm = __overload<_NaiveAlgorithm, _OptimizedAlgorithm>;
  return std::__unwrap_and_dispatch<_Algorithm>(std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2));
}

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___ALGORITHM_COPY_MOVE_COMMON_H
