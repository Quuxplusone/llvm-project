// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___RANGES_SIZE_H
#define _LIBCPP___RANGES_SIZE_H

#include <__concepts/arithmetic.h>
#include <__concepts/class_or_enum.h>
#include <__config>
#include <__iterator/concepts.h>
#include <__iterator/iterator_traits.h>
#include <__ranges/access.h>
#include <__type_traits/decay.h>
#include <__type_traits/enable_if.h>
#include <__type_traits/extent.h>
#include <__type_traits/is_bounded_array.h>
#include <__type_traits/is_unbounded_array.h>
#include <__type_traits/make_signed.h>
#include <__type_traits/make_unsigned.h>
#include <__type_traits/remove_cvref.h>
#include <__type_traits/remove_reference.h>
#include <__utility/auto_cast.h>
#include <__utility/declval.h>
#include <__utility/forward.h>
#include <__utility/priority_tag.h>
#include <cstddef>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

#if _LIBCPP_STD_VER >= 20

namespace ranges {
  template<class>
  inline constexpr bool disable_sized_range = false;
} // namespace ranges

// [range.prim.size]

namespace ranges {
namespace __size {
  void size(auto&) = delete;
  void size(const auto&) = delete;

  struct __fn {
    template <class _Tp>
      requires is_unbounded_array_v<remove_reference_t<_Tp>>
    _LIBCPP_HIDE_FROM_ABI
    static constexpr void __go(_Tp&&, __priority_tag<4>) = delete;

    template <class _Tp>
      requires is_bounded_array_v<remove_reference_t<_Tp>>
    _LIBCPP_HIDE_FROM_ABI
    static constexpr auto __go(_Tp&&, __priority_tag<3>)
      noexcept(noexcept(_LIBCPP_AUTO_CAST(extent_v<remove_reference_t<_Tp>>)))
      -> decltype(      _LIBCPP_AUTO_CAST(extent_v<remove_reference_t<_Tp>>))
      { return          _LIBCPP_AUTO_CAST(extent_v<remove_reference_t<_Tp>>); }

    template <class _Tp, enable_if_t<__class_or_enum<remove_reference_t<_Tp>>>* = nullptr>
      requires (!disable_sized_range<remove_cvref_t<_Tp>>)
    _LIBCPP_HIDE_FROM_ABI
    static constexpr auto __go(_Tp&& __t, __priority_tag<2>)
      noexcept(noexcept(_LIBCPP_AUTO_CAST(__t.size())))
      -> decltype(      _LIBCPP_AUTO_CAST(__t.size()))
      requires __integer_like<decltype(_LIBCPP_AUTO_CAST(__t.size()))>
      { return          _LIBCPP_AUTO_CAST(__t.size()); }

    template <class _Tp, enable_if_t<__class_or_enum<remove_reference_t<_Tp>>>* = nullptr>
      requires (!disable_sized_range<remove_cvref_t<_Tp>>)
    _LIBCPP_HIDE_FROM_ABI
    static constexpr auto __go(_Tp&& __t, __priority_tag<1>)
      noexcept(noexcept(_LIBCPP_AUTO_CAST(size(__t))))
      -> decltype(      _LIBCPP_AUTO_CAST(size(__t)))
      requires __integer_like<decltype(_LIBCPP_AUTO_CAST(size(__t)))>
      { return          _LIBCPP_AUTO_CAST(size(__t)); }

    template <class _Tp, class _It = decltype(ranges::begin(declval<_Tp&>()))>
    _LIBCPP_HIDE_FROM_ABI
    static constexpr auto __go(_Tp&& __t, __priority_tag<0>)
      noexcept(noexcept(std::__to_unsigned_like(ranges::end(__t) - ranges::begin(__t))))
      -> decltype(      std::__to_unsigned_like(ranges::end(__t) - ranges::begin(__t)))
      requires forward_iterator<_It> && sized_sentinel_for<decltype(ranges::end(__t)), _It>
      { return          std::__to_unsigned_like(ranges::end(__t) - ranges::begin(__t)); }

    template <class _Tp>
    [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr
    decltype(auto) operator()(_Tp&& __t) const
      noexcept(noexcept(__go(std::forward<_Tp>(__t), __priority_tag<4>())))
      requires (
        is_bounded_array_v<remove_reference_t<_Tp>> ||
        requires {
          requires __class_or_enum<remove_reference_t<_Tp>>;
          requires (
              requires { __t.size(); } ||
              requires { size(__t); }
          );
        } ||
        requires {
          ranges::end(__t) - ranges::begin(__t);
        }
      ) && requires {   __go(std::forward<_Tp>(__t), __priority_tag<4>()); }
      { return          __go(std::forward<_Tp>(__t), __priority_tag<4>()); }
  };
} // namespace __size

inline namespace __cpo {
  inline constexpr auto size = __size::__fn{};
} // namespace __cpo
} // namespace ranges

// [range.prim.ssize]

namespace ranges {
namespace __ssize {
  struct __fn {
    template<class _Tp>
      requires requires (_Tp&& __t) { ranges::size(__t); }
    [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr integral auto operator()(_Tp&& __t) const
      noexcept(noexcept(ranges::size(__t)))
    {
      using _Signed = make_signed_t<decltype(ranges::size(__t))>;
      if constexpr (sizeof(ptrdiff_t) > sizeof(_Signed))
        return static_cast<ptrdiff_t>(ranges::size(__t));
      else
        return static_cast<_Signed>(ranges::size(__t));
    }
  };
} // namespace __ssize

inline namespace __cpo {
  inline constexpr auto ssize = __ssize::__fn{};
} // namespace __cpo
} // namespace ranges

#endif // _LIBCPP_STD_VER >= 20

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___RANGES_SIZE_H
