// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___ALGORITHM_RANGES_EQUAL_H
#define _LIBCPP___ALGORITHM_RANGES_EQUAL_H

#include <__config>
#include <__iterator/algorithm_concepts.h>
#include <__iterator/concepts.h>
#include <__iterator/ranges_distance.h>
#include <__ranges/access.h>
#include <__ranges/concepts.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

#if !defined(_LIBCPP_HAS_NO_RANGES)

// [alg.equal]

namespace ranges {

namespace __equal {

  struct __fn {
    template<input_or_output_iterator _It1, sentinel_for<_It1> _Sent1,
             input_or_output_iterator _It2, sentinel_for<_It2> _Sent2,
             class _Pred = ranges::equal_to,
             class _Proj1 = identity, class _Proj2 = identity>
      requires indirectly_comparable<_It1, _It2, _Pred, _Proj1, _Proj2>
    constexpr bool operator()(_It1 __first1, _Sent1 __last1,
                              _It2 __first2, _Sent2 __last2,
                              _Pred __pred = {},
                              _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
      if constexpr (sized_sentinel_for<_Sent1, _It1> && sized_sentinel_for<_Sent2, _It2>) {
        if (ranges::distance(__first1, __last1) != ranges::distance(__first2, __last2))
          return false;
      }
      while (__first1 != __last1) {
        if (__first2 == __last2)
          return false;
        if (!bool(_VSTD::invoke(__pred, _VSTD::invoke(__proj1, *__first1), _VSTD::invoke(__proj2, *__first2))))
          return false;
        ++__first1;
        ++__first2;
      }
      return (__first2 == __last2);
    }

    template<input_range _Rp1, input_range _Rp2,
             class _Pred = ranges::equal_to,
             class _Proj1 = identity, class _Proj2 = identity>
      requires indirectly_comparable<iterator_t<_Rp1>, iterator_t<_Rp2>, _Pred, _Proj1, _Proj2>
    constexpr bool operator()(_Rp1&& __r1, _Rp2&& __r2,
                              _Pred __pred = {},
                              _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
      if constexpr (sized_range<_Rp1> && sized_range<_Rp2>) {
        if (ranges::distance(__r1) != ranges::distance(__r2))
          return false;
      }
      auto __first1 = ranges::begin(__r1);
      auto __last1 = ranges::end(__r1);
      auto __first2 = ranges::begin(__r2);
      auto __last2 = ranges::end(__r2);
      while (__first1 != __last1) {
        if (__first2 == __last2)
          return false;
        if (!bool(_VSTD::invoke(__pred, _VSTD::invoke(__proj1, *__first1), _VSTD::invoke(__proj2, *__first2))))
          return false;
        ++__first1;
        ++__first2;
      }
      return (__first2 == __last2);
    }
  };

} // namespace __equal

inline namespace __cpo {
  inline constexpr auto equal = __equal::__fn{};
} // namespace __cpo

} // namespace ranges

#endif // !defined(_LIBCPP_HAS_NO_RANGES)

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___ALGORITHM_RANGES_EQUAL_H
