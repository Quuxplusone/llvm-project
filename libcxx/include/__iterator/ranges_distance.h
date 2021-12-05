// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#ifndef _LIBCPP___ITERATOR_RANGES_DISTANCE_H
#define _LIBCPP___ITERATOR_RANGES_DISTANCE_H

#include <__config>
#include <__iterator/concepts.h>
#include <__iterator/incrementable_traits.h>
#include <__ranges/access.h>
#include <__ranges/concepts.h>
#include <__ranges/size.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

#if !defined(_LIBCPP_HAS_NO_RANGES)

// [range.iter.op.distance]

namespace ranges {

namespace __distance {

  struct __fn {
    template<input_or_output_iterator _It, sentinel_for<_It> _Sent>
      requires (!sized_sentinel_for<_Sent, _It>)
    constexpr iter_difference_t<_It> operator()(_It __first, _Sent __last) const {
      iter_difference_t<_It> __count = 0;
      while (__first != __last) {
        ++__first;
        ++__count;
      }
      return __count;
    }

    template<input_or_output_iterator _It, sized_sentinel_for<_It> _Sent>
    constexpr iter_difference_t<_It> operator()(_It __first, _Sent __last) const {
      return __last - __first;
    }

    template<range _Rp>
    constexpr range_difference_t<_Rp> operator()(_Rp&& __r) const {
      if constexpr (sized_range<_Rp>) {
        return static_cast<range_difference_t<_Rp>>(ranges::size(__r));
      } else {
        return (*this)(ranges::begin(__r), ranges::end(__r));
      }
    }
  };

} // namespace __distance

inline namespace __cpo {
  inline constexpr auto distance = __distance::__fn{};
} // namespace __cpo

} // namespace ranges

#endif // !defined(_LIBCPP_HAS_NO_RANGES)

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___ITERATOR_RANGES_DISTANCE_H
