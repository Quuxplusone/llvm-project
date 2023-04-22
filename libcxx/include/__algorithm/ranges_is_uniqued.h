//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP__ALGORITHM_RANGES_IS_UNIQUED_H
#define _LIBCPP__ALGORITHM_RANGES_IS_UNIQUED_H

#include <__algorithm/ranges_adjacent_find.h>
#include <__config>
#include <__functional/identity.h>
#include <__functional/ranges_operations.h>
#include <__iterator/concepts.h>
#include <__iterator/projected.h>
#include <__ranges/access.h>
#include <__ranges/concepts.h>
#include <__utility/move.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_PUSH_MACROS
#include <__undef_macros>

#if _LIBCPP_STD_VER >= 20

_LIBCPP_BEGIN_NAMESPACE_STD

namespace ranges {
namespace __is_uniqued {
struct __fn {
  template <forward_iterator _Iter, sentinel_for<_Iter> _Sent,
            class _Proj = identity,
            indirect_binary_predicate<projected<_Iter, _Proj>,
                                      projected<_Iter, _Proj>> _Pred = ranges::equal_to>
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr bool
  operator()(_Iter __first, _Sent __last, _Pred __pred = {}, _Proj __proj = {}) const {
    return ranges::__adjacent_find_impl(std::move(__first), __last, __pred, __proj) == __last;
  }

  template <forward_range _Range,
            class _Proj = identity,
            indirect_binary_predicate<projected<iterator_t<_Range>, _Proj>,
                                      projected<iterator_t<_Range>, _Proj>> _Pred = ranges::equal_to>
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr bool
  operator()(_Range&& __range, _Pred __pred = {}, _Proj __proj = {}) const {
    auto __last = ranges::end(__range);
    return ranges::__adjacent_find_impl(ranges::begin(__range), __last, __pred, __proj) == __last;
  }
};
} // namespace __is_uniqued

inline namespace __cpo {
  inline constexpr auto is_uniqued = __is_uniqued::__fn{};
} // namespace __cpo
} // namespace ranges

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP_STD_VER >= 20

_LIBCPP_POP_MACROS

#endif // _LIBCPP__ALGORITHM_RANGES_IS_UNIQUED_H
