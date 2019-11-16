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
#include <__memory/construct_at.h>
#include <__memory/voidify.h>
#include <__type_traits/is_destructible.h>
#include <__type_traits/is_move_constructible.h>
#include <__type_traits/is_nothrow_constructible.h>
#include <__type_traits/is_nothrow_move_constructible.h>
#include <__type_traits/is_trivially_destructible.h>
#include <__type_traits/is_trivially_relocatable.h>
#include <__type_traits/is_volatile.h>
#include <__type_traits/remove_cvref.h>
#include <__utility/move.h>
#include <cstring>
#include <new> // __launder

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

// relocate

template<class _Tp>
struct __destroy_guard {
    _Tp *__t_;
    _LIBCPP_HIDE_FROM_ABI explicit __destroy_guard(_Tp *__t) : __t_(__t) { }
    _LIBCPP_HIDE_FROM_ABI ~__destroy_guard() { std::__destroy_at(__t_); }
};

template<class _Tp, __enable_if_t<__libcpp_is_trivially_relocatable<_Tp>::value && !is_trivially_destructible<_Tp>::value, int> = 0>
_LIBCPP_NODISCARD _LIBCPP_HIDE_FROM_ABI
__remove_cvref_t<_Tp> relocate(_Tp *__source) _NOEXCEPT
{
    static_assert(is_move_constructible<_Tp>::value && is_destructible<_Tp>::value,
        "return std::move(source) must be well-formed");

    // This works on the Itanium ABI, because non-trivially-destructible objects
    // are always returned by hidden pointer, which happens to be the first argument.
    auto __more_magic = (__remove_cvref_t<_Tp>(*)(const void*, size_t))memcpy;
    return __more_magic(__source, sizeof(_Tp));
}

template<class _Tp, __enable_if_t<is_trivially_destructible<_Tp>::value || !__libcpp_is_trivially_relocatable<_Tp>::value, int> = 0>
_LIBCPP_NODISCARD _LIBCPP_HIDE_FROM_ABI
__remove_cvref_t<_Tp> relocate(_Tp *__source)
    _NOEXCEPT_(is_nothrow_move_constructible<_Tp>::value)
{
    // We need compiler magic to handle types which are trivially relocatable,
    // especially if they have throwing move-constructors.
    __destroy_guard<_Tp> __g(__source);
    return std::move(*__source);
}

// relocate_at

template<class _St, class _Dt>
_LIBCPP_HIDE_FROM_ABI
_Dt *__libcpp_relocate_at2(long, _St *__source, _Dt *__dest)
    _NOEXCEPT_((is_nothrow_constructible<_Dt, _St&&>::value))
{
    __destroy_guard<_St> __g(__source);
    return ::new (static_cast<void*>(__dest)) _Dt(std::move(*__source));
}

template<class _St, class _Dt, __enable_if_t<
    __is_same_uncvref<_St, _Dt>::value &&
    __libcpp_is_trivially_relocatable<__remove_cvref_t<_Dt> >::value &&
    !is_volatile<_St>::value && !is_volatile<_Dt>::value,
    int> = 0>
_LIBCPP_HIDE_FROM_ABI
_Dt *__libcpp_relocate_at2(int, _St *__source, _Dt *__dest) _NOEXCEPT {
    ::__builtin_memmove(__dest, __source, sizeof (_St));
    return std::__launder(__dest);
}

template<class _Tp>
_LIBCPP_HIDE_FROM_ABI
_Tp *relocate_at(_Tp *__source, _Tp *__dest)
    _NOEXCEPT_(noexcept(std::__libcpp_relocate_at2(0, __source, __dest)))
{
    static_assert(is_move_constructible<_Tp>::value && is_destructible<_Tp>::value,
        "::new (voidify(*dest)) T(std::move(*source)) must be well-formed");

    return std::__libcpp_relocate_at2(0, __source, __dest);
}

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___MEMORY_RELOCATE_AT_H
