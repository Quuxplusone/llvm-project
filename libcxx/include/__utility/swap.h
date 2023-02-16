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
#include <__type_traits/is_constant_evaluated.h>
#include <__type_traits/is_empty.h>
#include <__type_traits/is_move_assignable.h>
#include <__type_traits/is_move_constructible.h>
#include <__type_traits/is_nothrow_move_assignable.h>
#include <__type_traits/is_nothrow_move_constructible.h>
#include <__type_traits/is_swappable.h>
#include <__type_traits/is_trivially_relocatable.h>
#include <__type_traits/is_volatile.h>
#include <__utility/declval.h>
#include <__utility/move.h>
#include <cstddef>

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

#ifndef _LIBCPP_TRIVIAL_SWAP_SIZE
#  define _LIBCPP_TRIVIAL_SWAP_SIZE 128
#endif

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY __swap_result_t<_Tp> _LIBCPP_CONSTEXPR_SINCE_CXX20 swap(_Tp& __x, _Tp& __y)
    _NOEXCEPT_(is_nothrow_move_constructible<_Tp>::value && is_nothrow_move_assignable<_Tp>::value)
{
    if (__libcpp_is_trivially_relocatable<_Tp>::value && !is_volatile<_Tp>::value && !__libcpp_is_constant_evaluated()) {
        // If either __x or __y is a "possibly overlapping subobject"
        // in Itanium ABI terms, this will memcpy too much data. But the
        // same caveat has applied to std::copy and friends, since forever.
        char *__px = (char *)std::addressof(__x);
        char *__py = (char *)std::addressof(__y);
        if (is_empty<_Tp>::value || __px == __py) {
            // swap nothing
        } else if (_LIBCPP_TRIVIAL_SWAP_SIZE != 0 && sizeof(_Tp) > 128) {
            char __t[_LIBCPP_TRIVIAL_SWAP_SIZE];
            size_t __i = sizeof(_Tp);
            while (__i >= _LIBCPP_TRIVIAL_SWAP_SIZE) {
                ::__builtin_memcpy(__t, __px, _LIBCPP_TRIVIAL_SWAP_SIZE);
                ::__builtin_memcpy(__px, __py, _LIBCPP_TRIVIAL_SWAP_SIZE);
                ::__builtin_memcpy(__py, __t, _LIBCPP_TRIVIAL_SWAP_SIZE);
                __px += _LIBCPP_TRIVIAL_SWAP_SIZE;
                __py += _LIBCPP_TRIVIAL_SWAP_SIZE;
                __i -= _LIBCPP_TRIVIAL_SWAP_SIZE;
            }
            if (__i != 0) {
                ::__builtin_memcpy(__t, __px, __i);
                ::__builtin_memcpy(__px, __py, __i);
                ::__builtin_memcpy(__py, __t, __i);
            }
        } else {
            char __t[sizeof(_Tp)];
            ::__builtin_memcpy(__t, std::addressof(__x), sizeof(_Tp));
            ::__builtin_memcpy(std::addressof(__x), std::addressof(__y), sizeof(_Tp));
            ::__builtin_memcpy(std::addressof(__y), __t, sizeof(_Tp));
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
