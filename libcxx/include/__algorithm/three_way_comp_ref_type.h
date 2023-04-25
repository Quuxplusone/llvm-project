//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___ALGORITHM_THREE_WAY_COMP_REF_TYPE_H
#define _LIBCPP___ALGORITHM_THREE_WAY_COMP_REF_TYPE_H

#include <__compare/ordering.h>
#include <__config>
#include <__utility/declval.h>
#include <__utility/forward.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

#if _LIBCPP_STD_VER >= 20

template <class _Comp>
struct __debug_three_way_comp {
  _Comp& __comp_;
  _LIBCPP_HIDE_FROM_ABI constexpr __debug_three_way_comp(_Comp& __c) : __comp_(__c) {}

  template <class _Tp, class _Up>
  _LIBCPP_HIDE_FROM_ABI constexpr auto operator()(_Tp&& __x, _Up&& __y) const {
    auto __r = __comp_(__x, __y);
    if constexpr (__comparison_category<decltype(__r)>) {
      __do_compare_assert(__y, __x, __r);
    }
    return __r;
  }

  template <class _LHS, class _RHS>
  _LIBCPP_HIDE_FROM_ABI constexpr inline void __do_compare_assert(_LHS& __l, _RHS& __r, partial_ordering __o) const {
    auto __expected = __o;
    if (__o == partial_ordering::less)
      __expected = partial_ordering::greater;
    else if (__o == partial_ordering::greater)
      __expected = partial_ordering::less;
    _LIBCPP_ASSERT_UNCATEGORIZED(__comp_(__l, __r) == __expected, "Comparator does not induce a strict weak ordering");
    (void)__l;
    (void)__r;
    (void)__expected;
  }
};

// Pass the comparator by lvalue reference. Or in debug mode, using a
// debugging wrapper that stores a reference.
#  if _LIBCPP_HARDENING_MODE == _LIBCPP_HARDENING_MODE_DEBUG
template <class _Comp>
using __three_way_comp_ref_type = __debug_three_way_comp<_Comp>;
#  else
template <class _Comp>
using __three_way_comp_ref_type = _Comp&;
#  endif

#endif // _LIBCPP_STD_VER >= 20

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___ALGORITHM_THREE_WAY_COMP_REF_TYPE_H
