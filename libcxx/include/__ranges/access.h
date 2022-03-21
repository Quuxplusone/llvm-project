// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___RANGES_ACCESS_H
#define _LIBCPP___RANGES_ACCESS_H

#include <__concepts/class_or_enum.h>
#include <__config>
#include <__iterator/concepts.h>
#include <__iterator/readable_traits.h>
#include <__ranges/enable_borrowed_range.h>
#include <__type_traits/decay.h>
#include <__type_traits/enable_if.h>
#include <__type_traits/extent.h>
#include <__type_traits/is_array.h>
#include <__type_traits/is_bounded_array.h>
#include <__type_traits/is_reference.h>
#include <__type_traits/is_unbounded_array.h>
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
template <class _Tp>
concept __can_borrow = is_lvalue_reference_v<_Tp> || enable_borrowed_range<remove_cvref_t<_Tp>>;
} // namespace ranges

// [range.access.begin]

namespace ranges {
namespace __begin {
void begin() = delete;

struct __fn {
  template <class _Tp>
    requires is_rvalue_reference_v<_Tp&&> && (!enable_borrowed_range<remove_cvref_t<_Tp>>)
  _LIBCPP_HIDE_FROM_ABI
  static constexpr void __go(_Tp&&, __priority_tag<3>) = delete;

  template <class _Tp, enable_if_t<is_array_v<remove_reference_t<_Tp>>>* = nullptr>
  _LIBCPP_HIDE_FROM_ABI
  static constexpr auto __go(_Tp&& __t, __priority_tag<2>)
    noexcept(noexcept(__t + 0))
    -> decltype(      __t + 0)
    { return          __t + 0; }

  template <class _Tp, enable_if_t<__class_or_enum<remove_reference_t<_Tp>>>* = nullptr>
  _LIBCPP_HIDE_FROM_ABI
  static constexpr auto __go(_Tp&& __t, __priority_tag<1>)
    noexcept(noexcept(_LIBCPP_AUTO_CAST(__t.begin())))
    -> decltype(      _LIBCPP_AUTO_CAST(__t.begin()))
    requires input_or_output_iterator<decltype(_LIBCPP_AUTO_CAST(__t.begin()))>
    { return          _LIBCPP_AUTO_CAST(__t.begin()); }

  template <class _Tp, enable_if_t<__class_or_enum<remove_reference_t<_Tp>>>* = nullptr>
  _LIBCPP_HIDE_FROM_ABI
  static constexpr auto __go(_Tp&& __t, __priority_tag<0>)
    noexcept(noexcept(_LIBCPP_AUTO_CAST(begin(__t))))
    -> decltype(      _LIBCPP_AUTO_CAST(begin(__t)))
    requires input_or_output_iterator<decltype(_LIBCPP_AUTO_CAST(begin(__t)))>
    { return          _LIBCPP_AUTO_CAST(begin(__t)); }

  template <class _Tp>
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr
  decltype(auto) operator()(_Tp&& __t) const
    noexcept(noexcept(__go(std::forward<_Tp>(__t), __priority_tag<3>())))
    requires (
      requires {
        requires __class_or_enum<remove_cvref_t<_Tp>>;
        requires (
          requires {
            __t.begin();
            { _LIBCPP_AUTO_CAST(__t.begin()) } -> input_or_output_iterator;
          } ||
          requires {
            begin(__t);
            { _LIBCPP_AUTO_CAST(begin(__t)) } -> input_or_output_iterator;
          }
        );
      } ||
      is_array_v<remove_reference_t<_Tp>>
    ) && requires {   __go(std::forward<_Tp>(__t), __priority_tag<3>()); }
    { return          __go(std::forward<_Tp>(__t), __priority_tag<3>()); }
  };
} // namespace __begin

inline namespace __cpo {
inline constexpr auto begin = __begin::__fn{};
} // namespace __cpo
} // namespace ranges

// [range.range]

namespace ranges {
template <class _Tp>
using iterator_t = decltype(ranges::begin(std::declval<_Tp&>()));
} // namespace ranges

// [range.access.end]

namespace ranges {
namespace __end {
void end() = delete;

struct __fn {
  template <class _Tp>
    requires is_rvalue_reference_v<_Tp&&> && (!enable_borrowed_range<remove_cvref_t<_Tp>>)
  _LIBCPP_HIDE_FROM_ABI
  static constexpr void __go(_Tp&&, __priority_tag<4>) = delete;

  template <class _Tp>
    requires is_unbounded_array_v<remove_reference_t<_Tp>>
  _LIBCPP_HIDE_FROM_ABI
  static constexpr void __go(_Tp&&, __priority_tag<3>) = delete;

  template <class _Tp, enable_if_t<is_bounded_array_v<remove_reference_t<_Tp>>>* = nullptr>
  _LIBCPP_HIDE_FROM_ABI
  static constexpr auto __go(_Tp&& __t, __priority_tag<2>)
    noexcept(noexcept(__t + extent_v<remove_reference_t<_Tp>>))
    -> decltype(      __t + extent_v<remove_reference_t<_Tp>>)
    { return          __t + extent_v<remove_reference_t<_Tp>>; }

  template <class _Tp, enable_if_t<__class_or_enum<remove_reference_t<_Tp>>>* = nullptr>
  _LIBCPP_HIDE_FROM_ABI
  static constexpr auto __go(_Tp&& __t, __priority_tag<1>)
    noexcept(noexcept(_LIBCPP_AUTO_CAST(__t.end())))
    -> decltype(      _LIBCPP_AUTO_CAST(__t.end()))
    requires sentinel_for<decltype(_LIBCPP_AUTO_CAST(__t.end())), iterator_t<remove_reference_t<_Tp>>>
    { return          _LIBCPP_AUTO_CAST(__t.end()); }

  template <class _Tp, enable_if_t<__class_or_enum<remove_reference_t<_Tp>>>* = nullptr>
  _LIBCPP_HIDE_FROM_ABI
  static constexpr auto __go(_Tp&& __t, __priority_tag<0>)
    noexcept(noexcept(_LIBCPP_AUTO_CAST(end(__t))))
    -> decltype(      _LIBCPP_AUTO_CAST(end(__t)))
    requires sentinel_for<decltype(_LIBCPP_AUTO_CAST(end(__t))), iterator_t<remove_reference_t<_Tp>>>
    { return          _LIBCPP_AUTO_CAST(end(__t)); }

  template <class _Tp>
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr
  decltype(auto) operator()(_Tp&& __t) const
    noexcept(noexcept(__go(std::forward<_Tp>(__t), __priority_tag<4>())))
    requires (
      requires {
        requires __class_or_enum<remove_cvref_t<_Tp>>;
        requires (
          requires {
            __t.end();
            { _LIBCPP_AUTO_CAST(__t.end()) } -> sentinel_for<iterator_t<remove_reference_t<_Tp>>>;
          } ||
          requires {
            end(__t);
            { _LIBCPP_AUTO_CAST(end(__t)) } -> sentinel_for<iterator_t<remove_reference_t<_Tp>>>;
          }
        );
      } ||
      is_bounded_array_v<remove_reference_t<_Tp>>
    ) && requires {   __go(std::forward<_Tp>(__t), __priority_tag<4>()); }
    { return          __go(std::forward<_Tp>(__t), __priority_tag<4>()); }
  };
} // namespace __end

inline namespace __cpo {
inline constexpr auto end = __end::__fn{};
} // namespace __cpo
} // namespace ranges

// [range.access.cbegin]

namespace ranges {
namespace __cbegin {
struct __fn {
  template <class _Tp>
    requires is_lvalue_reference_v<_Tp&&>
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr auto operator()(_Tp&& __t) const
      noexcept(noexcept(ranges::begin(static_cast<const remove_reference_t<_Tp>&>(__t))))
          -> decltype(ranges::begin(static_cast<const remove_reference_t<_Tp>&>(__t))) {
    return ranges::begin(static_cast<const remove_reference_t<_Tp>&>(__t));
  }

  template <class _Tp>
    requires is_rvalue_reference_v<_Tp&&>
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr auto operator()(_Tp&& __t) const
      noexcept(noexcept(ranges::begin(static_cast<const _Tp&&>(__t))))
          -> decltype(ranges::begin(static_cast<const _Tp&&>(__t))) {
    return ranges::begin(static_cast<const _Tp&&>(__t));
  }
};
} // namespace __cbegin

inline namespace __cpo {
inline constexpr auto cbegin = __cbegin::__fn{};
} // namespace __cpo
} // namespace ranges

// [range.access.cend]

namespace ranges {
namespace __cend {
struct __fn {
  template <class _Tp>
    requires is_lvalue_reference_v<_Tp&&>
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr auto operator()(_Tp&& __t) const
      noexcept(noexcept(ranges::end(static_cast<const remove_reference_t<_Tp>&>(__t))))
          -> decltype(ranges::end(static_cast<const remove_reference_t<_Tp>&>(__t))) {
    return ranges::end(static_cast<const remove_reference_t<_Tp>&>(__t));
  }

  template <class _Tp>
    requires is_rvalue_reference_v<_Tp&&>
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr auto operator()(_Tp&& __t) const
      noexcept(noexcept(ranges::end(static_cast<const _Tp&&>(__t))))
          -> decltype(ranges::end(static_cast<const _Tp&&>(__t))) {
    return ranges::end(static_cast<const _Tp&&>(__t));
  }
};
} // namespace __cend

inline namespace __cpo {
inline constexpr auto cend = __cend::__fn{};
} // namespace __cpo
} // namespace ranges

#endif // _LIBCPP_STD_VER >= 20

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___RANGES_ACCESS_H
