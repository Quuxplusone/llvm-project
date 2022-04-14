//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___MEMORY_RELOCATE_AT_H
#define _LIBCPP___MEMORY_RELOCATE_AT_H

#include <__config>
#include <__utility/move.h>
#include <cstring> // memmove
#include <new> // __launder
#include <type_traits>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

// __libcpp_relocate

template<class _Tp>
struct __destroy_guard {
    _Tp& __t_;
    explicit __destroy_guard(_Tp& __t) : __t_(__t) { }
    ~__destroy_guard() { __t_.~_Tp(); }
};

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp __libcpp_relocate(_Tp *__source)
    _NOEXCEPT_(is_nothrow_move_constructible<_Tp>::value)
{
    // We need compiler magic to handle types which are trivially relocatable,
    // especially if they have throwing move-constructors.
    __destroy_guard<_Tp> __g(*__source);
    return _VSTD::move(*__source);
}

// __libcpp_relocate_at

template<class _St, class _Dt>
inline _LIBCPP_INLINE_VISIBILITY
_Dt *__libcpp_relocate_at2(long, _St *__source, _Dt *__dest)
    _NOEXCEPT_((is_nothrow_constructible<_Dt, _St&&>::value))
{
    __destroy_guard<_St> __g(*__source);
    return ::new (static_cast<void*>(__dest)) _Dt(_VSTD::move(*__source));
}

template<class _St, class _Dt, __enable_if_t<
    __is_same_uncvref<_St, _Dt>::value &&
    __libcpp_is_trivially_relocatable<__uncvref_t<_Dt> >::value &&
    !is_volatile<_St>::value && !is_volatile<_Dt>::value,
    int> = 0>
inline _LIBCPP_INLINE_VISIBILITY
_Dt *__libcpp_relocate_at2(int, _St *__source, _Dt *__dest) _NOEXCEPT {
    _VSTD::memmove(__dest, __source, sizeof (_St));
    return _VSTD::__launder(__dest);
}

template<class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_Tp *__libcpp_relocate_at(_Tp *__source, _Tp *__dest)
    _NOEXCEPT_(noexcept(_VSTD::__libcpp_relocate_at2(0, __source, __dest)))
{
    return _VSTD::__libcpp_relocate_at2(0, __source, __dest);
}

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___MEMORY_RELOCATE_AT_H
