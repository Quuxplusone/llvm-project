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
#include <__type_traits/enable_if.h>
#include <__type_traits/is_always_bitcastable.h>
#include <__type_traits/is_constant_evaluated.h>
#include <__type_traits/is_copy_constructible.h>
#include <__type_traits/is_trivially_assignable.h>
#include <__type_traits/is_trivially_copyable.h>
#include <__type_traits/is_volatile.h>
#include <__utility/move.h>
#include <__utility/pair.h>
#include <cstddef>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

// Type traits.

template <class _IterOps, class _From, class _ToIter, class = void>
struct __can_copy_without_conversion : false_type {};

template <class _IterOps, class _From, class _ToIter>
struct __can_copy_without_conversion<
    _IterOps,
    _From,
    _ToIter,
    __enable_if_t<is_same<_From, typename _IterOps::template __value_type<_ToIter> >::value> > : true_type {};

template <class _OptimizedAlgorithm, class _IterOps, class _FromIter, class _ToIter>
struct __can_lower_to_memmove_in_consteval {
#ifdef _LIBCPP_COMPILER_GCC
  // GCC doesn't support `__builtin_memmove` during constant evaluation.
  static const bool value = false;
#else
  // In Clang, `__builtin_memmove` only supports fully trivially copyable types (just having trivial copy assignment is
  // insufficient). Also, conversions are not supported.
  // _FromIter always has a value_type, but _ToIter might not; hence the template indirection here.
  using _From = typename _IterOps::template __value_type<_FromIter>;
  static const bool value =
      is_trivially_copyable<_From>::value && __can_copy_without_conversion<_IterOps, _From, _ToIter>::value;
#endif
};

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

// `memmove` algorithms implementation.

template <class _In, class _Out>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX14 pair<_In*, _Out*>
__copy_trivial_impl(_In* __first, _In* __last, _Out* __result) {
  const size_t __n = static_cast<size_t>(__last - __first);
  ::__builtin_memmove(__result, __first, __n * sizeof(_Out));

  return std::make_pair(__last, __result + __n);
}

template <class _In, class _Out>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX14 pair<_In*, _Out*>
__copy_backward_trivial_impl(_In* __first, _In* __last, _Out* __result) {
  const size_t __n = static_cast<size_t>(__last - __first);
  __result -= __n;

  ::__builtin_memmove(__result, __first, __n * sizeof(_Out));

  return std::make_pair(__last, __result);
}

// Iterator unwrapping and dispatching to the correct overload.

template <class _F1, class _F2>
struct __overload : _F1, _F2 {
  using _F1::operator();
  using _F2::operator();
};

template <class _Iter, class = void>
struct __can_rewrap : false_type {};

template <class _Iter>
struct __can_rewrap<_Iter,
                    __enable_if_t<is_copy_constructible<_Iter>::value> > : true_type {};

template <class _Algorithm,
          class _Iter1,
          class _Sent1,
          class _Iter2,
          __enable_if_t<__can_rewrap<_Iter1>::value && __can_rewrap<_Iter2>::value, int> = 0>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX17 pair<_Iter1, _Iter2>
__unwrap_and_dispatch(_Iter1 __first1, _Sent1 __last1, _Iter2 __first2) {
  auto __range  = std::__unwrap_range(__first1, std::move(__last1));
  auto __result = _Algorithm()(std::move(__range.first), std::move(__range.second), std::__unwrap_iter(__first2));
  return std::make_pair(std::__rewrap_range<_Sent1>(std::move(__first1), std::move(__result.first)),
                                 std::__rewrap_iter(std::move(__first2), std::move(__result.second)));
}

template <class _Algorithm,
          class _Iter1,
          class _Sent1,
          class _Iter2,
          __enable_if_t<!__can_rewrap<_Iter1>::value || !__can_rewrap<_Iter2>::value, int> = 0>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX17 pair<_Iter1, _Iter2>
__unwrap_and_dispatch(_Iter1 __first1, _Sent1 __last1, _Iter2 __first2) {
  return _Algorithm()(std::move(__first1), std::move(__last1), std::move(__first2));
}

template <class _AlgPolicy,
          class _NaiveAlgorithm,
          class _OptimizedAlgorithm,
          class _Iter1,
          class _Sent1,
          class _Iter2>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX17 pair<_Iter1, _Iter2>
__dispatch_copy_or_move(_Iter1 __first1, _Sent1 __last1, _Iter2 __first2) {
  if (__libcpp_is_constant_evaluated() && !__can_lower_to_memmove_in_consteval<_OptimizedAlgorithm, _IterOps<_AlgPolicy>, _Iter1, _Iter2>::value) {
    return std::__unwrap_and_dispatch<_NaiveAlgorithm>(std::move(__first1), std::move(__last1), std::move(__first2));
  }
  using _Algorithm = __overload<_NaiveAlgorithm, _OptimizedAlgorithm>;
  return std::__unwrap_and_dispatch<_Algorithm>(std::move(__first1), std::move(__last1), std::move(__first2));
}

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___ALGORITHM_COPY_MOVE_COMMON_H
