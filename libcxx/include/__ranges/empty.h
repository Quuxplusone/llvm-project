// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___RANGES_EMPTY_H
#define _LIBCPP___RANGES_EMPTY_H

#include <__concepts/class_or_enum.h>
#include <__config>
#include <__iterator/concepts.h>
#include <__ranges/access.h>
#include <__ranges/size.h>
#include <__type_traits/enable_if.h>
#include <__type_traits/remove_reference.h>
#include <__utility/declval.h>
#include <__utility/forward.h>
#include <__utility/priority_tag.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

#if _LIBCPP_STD_VER > 17

// [range.prim.empty]

namespace ranges {
namespace __empty {
  struct __fn {
    template <class _Tp, enable_if_t<__class_or_enum<remove_reference_t<_Tp>>>* = nullptr>
    _LIBCPP_HIDE_FROM_ABI
    static constexpr auto __go(_Tp&& __t, __priority_tag<2>)
      noexcept(noexcept(bool(__t.empty())))
      -> decltype(      bool(__t.empty()))
      { return          bool(__t.empty()); }

    template <class _Tp>
    _LIBCPP_HIDE_FROM_ABI
    static constexpr auto __go(_Tp&& __t, __priority_tag<1>)
      noexcept(noexcept(ranges::size(__t) == 0))
      -> decltype(      ranges::size(__t) == 0)
      { return          ranges::size(__t) == 0; }

    template <class _Tp, class _It = decltype(ranges::begin(declval<_Tp&>()))>
    _LIBCPP_HIDE_FROM_ABI
    static constexpr auto __go(_Tp&& __t, __priority_tag<0>)
      noexcept(noexcept(bool(ranges::begin(__t) == ranges::end(__t))))
      -> decltype(      bool(ranges::begin(__t) == ranges::end(__t)))
      requires forward_iterator<_It>
      { return          bool(ranges::begin(__t) == ranges::end(__t)); }

    template <class _Tp>
    [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr
    decltype(auto) operator()(_Tp&& __t) const
      noexcept(noexcept(__go(std::forward<_Tp>(__t), __priority_tag<2>())))
      requires (
        requires {
          requires __class_or_enum<remove_reference_t<_Tp>>;
          __t.empty();
        } ||
        requires {
          ranges::size(__t) == 0;
        } ||
        requires {
          ranges::begin(__t) == ranges::end(__t);
          { ranges::begin(__t) } -> forward_iterator;
        }
      ) && requires {   __go(std::forward<_Tp>(__t), __priority_tag<2>()); }
      { return          __go(std::forward<_Tp>(__t), __priority_tag<2>()); }

  };
} // namespace __empty

inline namespace __cpo {
  inline constexpr auto empty = __empty::__fn{};
} // namespace __cpo
} // namespace ranges

#endif // _LIBCPP_STD_VER > 17

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___RANGES_EMPTY_H
