//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___UTILITY_SWAP_H
#define _LIBCPP___UTILITY_SWAP_H

#include <__config>
#include <__memory/addressof.h>
#include <__utility/declval.h>
#include <__utility/move.h>
#include <cstddef>
#include <cstring>
#include <type_traits>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

#ifndef _LIBCPP_CXX03_LANG
template <class _Tp>
using __swap_result_t = typename enable_if<is_move_constructible<_Tp>::value && is_move_assignable<_Tp>::value>::type;
#else
template <class>
using __swap_result_t = void;
#endif

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY __swap_result_t<_Tp> _LIBCPP_CONSTEXPR_SINCE_CXX20 swap(_Tp& __x, _Tp& __y)
    _NOEXCEPT_(is_nothrow_move_constructible<_Tp>::value && is_nothrow_move_assignable<_Tp>::value)
{
    if (__libcpp_is_trivially_relocatable<_Tp>::value && !is_volatile<_Tp>::value && !__libcpp_is_constant_evaluated()) {
        if (!is_empty<_Tp>::value) {
            // TODO: if either __x or __y is a "possibly overlapping subobject"
            // in Itanium ABI terms, this will memcpy too much data. But the
            // same caveat has applied to std::copy and friends, since forever.
            char __t[sizeof (_Tp)];
            // We cannot use __libcpp_relocate_at here, because the stack array
            // is deliberately underaligned for an actual object of type _Tp.
            // We are deliberately shuffling bytes here, not objects.
            std::memcpy(__t, std::addressof(__x), sizeof(_Tp));
            std::memcpy(std::addressof(__x), std::addressof(__y), sizeof(_Tp));
            std::memcpy(std::addressof(__y), __t, sizeof(_Tp));
        }
    } else {
        _Tp __t(std::move(__x));
        __x = std::move(__y);
        __y = std::move(__t);
    }
}

template <class _Tp, size_t _Np>
inline _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR_SINCE_CXX20 typename enable_if<__is_swappable<_Tp>::value>::type
swap(_Tp (&__a)[_Np], _Tp (&__b)[_Np]) _NOEXCEPT_(__is_nothrow_swappable<_Tp>::value) {
  for (size_t __i = 0; __i != _Np; ++__i) {
    swap(__a[__i], __b[__i]);
  }
}

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___UTILITY_SWAP_H
