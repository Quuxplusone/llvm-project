//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___ALGORITHM_SWAP_RANGES_H
#define _LIBCPP___ALGORITHM_SWAP_RANGES_H

#include <__algorithm/copy_move_common.h>
#include <__algorithm/iterator_operations.h>
#include <__config>
#include <__string/constexpr_c_functions.h>
#include <__type_traits/datasizeof.h>
#include <__type_traits/is_constant_evaluated.h>
#include <__utility/move.h>
#include <__utility/pair.h>
#include <__utility/swap.h>
#include <cstddef>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_PUSH_MACROS
#include <__undef_macros>

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _AlgPolicy, class _Tp, class _Up>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 void
__constexpr_memswap(_Tp* __first1, _Up* __first2, __element_count __n) {
  size_t __count = static_cast<size_t>(__n);
  if (__libcpp_is_constant_evaluated()) {
    for (size_t __i = 0; __i != __count; ++__i)
      _IterOps<_AlgPolicy>::iter_swap(__first1 + __i, __first2 + __i);
  } else if (__count > 0) {
    std::__libcpp_memswap(__first1, __first2, (__count - 1) * sizeof(_Tp) + __datasizeof_v<_Tp>);
  }
}

template <class _AlgPolicy, class _Tp, class _Up>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 pair<_Tp*, _Up*>
__swap_ranges_trivial_impl(_Tp* __first1, _Tp* __last1, _Up* __first2) {
  const size_t __n = static_cast<size_t>(__last1 - __first1);

  std::__constexpr_memswap<_AlgPolicy>(__first1, __first2, __element_count(__n));

  return std::make_pair(__last1, __first2 + __n);
}

template <class _AlgPolicy>
struct __swap_ranges_impl {
  // 2+2 iterators: the shorter size will be used.
  template <class _ForwardIterator1, class _Sentinel1, class _ForwardIterator2, class _Sentinel2>
  _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20
  pair<_ForwardIterator1, _ForwardIterator2>
  operator()(_ForwardIterator1 __first1, _Sentinel1 __last1, _ForwardIterator2 __first2, _Sentinel2 __last2) const {
    while (__first1 != __last1 && __first2 != __last2) {
      _IterOps<_AlgPolicy>::iter_swap(__first1, __first2);
      ++__first1;
      ++__first2;
    }
    return pair<_ForwardIterator1, _ForwardIterator2>(std::move(__first1), std::move(__first2));
  }

  // 2+1 iterators: size2 >= size1.
  template <class _ForwardIterator1, class _Sentinel1, class _ForwardIterator2>
  _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20
  pair<_ForwardIterator1, _ForwardIterator2>
  operator()(_ForwardIterator1 __first1, _Sentinel1 __last1, _ForwardIterator2 __first2) const {
    while (__first1 != __last1) {
      _IterOps<_AlgPolicy>::iter_swap(__first1, __first2);
      ++__first1;
      ++__first2;
    }
    return pair<_ForwardIterator1, _ForwardIterator2>(std::move(__first1), std::move(__first2));
  }

  template <class _Tp, class _Up,
            __enable_if_t<__can_lower_swap_to_memswap<_Tp, _Up>::value, int> = 0>
  _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 pair<_Tp*, _Up*>
  operator()(_Tp* __first1, _Tp* __last1, _Up* __first2) const {
    return std::__swap_ranges_trivial_impl<_AlgPolicy>(__first1, __last1, __first2);
  }

  template <class _Tp, class _Up,
            __enable_if_t<__can_lower_swap_to_memswap<_Tp, _Up>::value, int> = 0>
  _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 pair<_Tp*, _Up*>
  operator()(_Tp* __first1, _Tp* __last1, _Up* __first2, _Up* __last2) const {
    auto __n1 = __last1 - __first1;
    auto __n2 = __last2 - __first2;
    auto __n = (__n1 < __n2) ? __n1 : __n2;
    return std::__swap_ranges_trivial_impl<_AlgPolicy>(__first1, __first1 + __n, __first2);
  }
};

template <class _AlgPolicy, class _ForwardIterator1, class _Sentinel1, class _ForwardIterator2>
inline _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20
pair<_ForwardIterator1, _ForwardIterator2>
__swap_ranges(_ForwardIterator1 __first1, _Sentinel1 __last1, _ForwardIterator2 __first2) {
  if (__libcpp_is_constant_evaluated()) {
    return __swap_ranges_impl<_AlgPolicy>()(
        std::move(__first1), std::move(__last1), std::move(__first2));
  } else {
    return std::__copy_move_unwrap_iters<__swap_ranges_impl<_AlgPolicy> >(
        std::move(__first1), std::move(__last1), std::move(__first2));
  }
}

template <class _AlgPolicy, class _ForwardIterator1, class _Sentinel1, class _ForwardIterator2, class _Sentinel2>
inline _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20
pair<_ForwardIterator1, _ForwardIterator2>
__swap_ranges(_ForwardIterator1 __first1, _Sentinel1 __last1, _ForwardIterator2 __first2, _Sentinel2 __last2) {
  if (__libcpp_is_constant_evaluated()) {
    return __swap_ranges_impl<_AlgPolicy>()(
        std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2));
  } else {
    return std::__copy_move_unwrap_iters<__swap_ranges_impl<_AlgPolicy> >(
        std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2));
  }
}

template <class _ForwardIterator1, class _ForwardIterator2>
inline _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 _ForwardIterator2
swap_ranges(_ForwardIterator1 __first1, _ForwardIterator1 __last1, _ForwardIterator2 __first2) {
  return std::__swap_ranges<_ClassicAlgPolicy>(std::move(__first1), std::move(__last1), std::move(__first2)).second;
}

_LIBCPP_END_NAMESPACE_STD

_LIBCPP_POP_MACROS

#endif // _LIBCPP___ALGORITHM_SWAP_RANGES_H
