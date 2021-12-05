// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#ifndef _LIBCPP___RANGES_TAKE_H
#define _LIBCPP___RANGES_TAKE_H

#include <__algorithm/min.h>
#include <__concepts/constructible.h>
#include <__config>
#include <__functional/bind_back.h>
#include <__iterator/ranges_distance.h>
#include <__ranges/access.h>
#include <__ranges/concepts.h>
#include <__ranges/empty_view.h>
#include <__ranges/iota_view.h>
#include <__ranges/range_adaptor.h>
#include <__ranges/size.h>
#include <__ranges/subrange.h>
#include <__ranges/take_view.h>
#include <__utility/forward.h>
#include <__utility/priority_tag.h>
#include <concepts>
#include <span>
#include <string_view>
#include <type_traits>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

_LIBCPP_PUSH_MACROS
#include <__undef_macros>

_LIBCPP_BEGIN_NAMESPACE_STD

#if !defined(_LIBCPP_HAS_NO_RANGES)

namespace ranges::views {

namespace __take {

  template<class> struct __is_iota_view : false_type {};
  template<class _Start, class _Bound> struct __is_iota_view<iota_view<_Start, _Bound>> : true_type {};

  template<class> struct __is_string_view : false_type {};
  template<class _CharT, class _Traits> struct __is_string_view<basic_string_view<_CharT, _Traits>> : true_type {};

  template<class> struct __is_subrange : false_type {};
  template<class _Ip, class _Sp, ranges::subrange_kind _Kp> struct __is_subrange<subrange<_Ip, _Sp, _Kp>> : true_type {};

  struct __fn : __range_adaptor_closure<__fn> {

    template<class _It, class _Dp>
    _LIBCPP_HIDE_FROM_ABI
    static constexpr auto __make_iota_view(_It __first, _Dp __n, type_identity_t<_Dp> __count)
        noexcept(noexcept(ranges::iota_view(*__first, *(__first + _VSTD::min(__n, __count)))))
        -> decltype(      ranges::iota_view(*__first, *(__first + _VSTD::min(__n, __count))))
        { return          ranges::iota_view(*__first, *(__first + _VSTD::min(__n, __count))); }

    template<class _Result, class _It, class _Dp>
    _LIBCPP_HIDE_FROM_ABI
    static constexpr auto __make_result(_It __first, _Dp __n, type_identity_t<_Dp> __count)
        noexcept(noexcept(_Result(__first, __first + _VSTD::min(__n, __count))))
        -> decltype(      _Result(__first, __first + _VSTD::min(__n, __count)))
        { return          _Result(__first, __first + _VSTD::min(__n, __count)); }

    template<class _Rp, class _Count>
      requires __is_empty_view<decay_t<_Rp>>::value
    _LIBCPP_HIDE_FROM_ABI
    static constexpr decay_t<_Rp> __go(_Rp&& __r, _Count&&) noexcept {
      return _VSTD::forward<_Rp>(__r);
    }

    template<class _Rp, class _Count>
      requires random_access_range<_Rp> && sized_range<_Rp> && __is_std_span<decay_t<_Rp>>::value
    _LIBCPP_HIDE_FROM_ABI
    static constexpr auto __go(_Rp&& __r, _Count&& __count)
      noexcept(noexcept(__make_result<span<typename decay_t<_Rp>::element_type>>(ranges::begin(__r), ranges::distance(__r), __count)))
      -> decltype(      __make_result<span<typename decay_t<_Rp>::element_type>>(ranges::begin(__r), ranges::distance(__r), __count))
      { return          __make_result<span<typename decay_t<_Rp>::element_type>>(ranges::begin(__r), ranges::distance(__r), __count); }

    template<class _Rp, class _Count>
      requires random_access_range<_Rp> && sized_range<_Rp> && __is_string_view<decay_t<_Rp>>::value
    _LIBCPP_HIDE_FROM_ABI
    static constexpr auto __go(_Rp&& __r, _Count&& __count)
      noexcept(noexcept(__make_result<decay_t<_Rp>>(ranges::begin(__r), ranges::distance(__r), __count)))
      -> decltype(      __make_result<decay_t<_Rp>>(ranges::begin(__r), ranges::distance(__r), __count))
      { return          __make_result<decay_t<_Rp>>(ranges::begin(__r), ranges::distance(__r), __count); }

    template<class _Rp, class _Count>
      requires random_access_range<_Rp> && sized_range<_Rp> && __is_subrange<decay_t<_Rp>>::value
    _LIBCPP_HIDE_FROM_ABI
    static constexpr auto __go(_Rp&& __r, _Count&& __count)
      noexcept(noexcept(__make_result<subrange<iterator_t<decay_t<_Rp>>>>(ranges::begin(__r), ranges::distance(__r), __count)))
      -> decltype(      __make_result<subrange<iterator_t<decay_t<_Rp>>>>(ranges::begin(__r), ranges::distance(__r), __count))
      { return          __make_result<subrange<iterator_t<decay_t<_Rp>>>>(ranges::begin(__r), ranges::distance(__r), __count); }

    template<class _Rp, class _Count>
      requires random_access_range<_Rp> && sized_range<_Rp> && __is_iota_view<decay_t<_Rp>>::value
    _LIBCPP_HIDE_FROM_ABI
    static constexpr auto __go(_Rp&& __r, _Count&& __count)
      noexcept(noexcept(__make_iota_view(ranges::begin(__r), ranges::distance(__r), __count)))
      -> decltype(      __make_iota_view(ranges::begin(__r), ranges::distance(__r), __count))
      { return          __make_iota_view(ranges::begin(__r), ranges::distance(__r), __count); }

    template<class _Rp, class _Count>
    _LIBCPP_HIDE_FROM_ABI
    static constexpr auto __go(_Rp&& __r, _Count&& __count)
      noexcept(noexcept(ranges::take_view(_VSTD::forward<_Rp>(__r), _VSTD::forward<_Count>(__count))))
      -> decltype(      ranges::take_view(_VSTD::forward<_Rp>(__r), _VSTD::forward<_Count>(__count)))
      { return          ranges::take_view(_VSTD::forward<_Rp>(__r), _VSTD::forward<_Count>(__count)); }

    template<class _Rp, convertible_to<range_difference_t<_Rp&&>> _Count>
    [[nodiscard]] _LIBCPP_HIDE_FROM_ABI
    constexpr auto operator()(_Rp&& __r, _Count&& __count) const
      noexcept(noexcept(__go(_VSTD::forward<_Rp>(__r), _VSTD::forward<_Count>(__count))))
      -> decltype(      __go(_VSTD::forward<_Rp>(__r), _VSTD::forward<_Count>(__count)))
      { return          __go(_VSTD::forward<_Rp>(__r), _VSTD::forward<_Count>(__count)); }

    template<class _Count>
      requires constructible_from<decay_t<_Count>, _Count>
    [[nodiscard]] _LIBCPP_HIDE_FROM_ABI
    constexpr auto operator()(_Count&& __count) const
      noexcept(is_nothrow_constructible_v<decay_t<_Count>, _Count>)
    { return __range_adaptor_closure_t(_VSTD::__bind_back(*this, _VSTD::forward<_Count>(__count))); }
  };

} // namespace __take

inline namespace __cpo {
  inline constexpr auto take = __take::__fn{};
} // namespace __cpo

} // namespace ranges::views

#endif // !defined(_LIBCPP_HAS_NO_RANGES)

_LIBCPP_END_NAMESPACE_STD

_LIBCPP_POP_MACROS

#endif // _LIBCPP___RANGES_TAKE_H
