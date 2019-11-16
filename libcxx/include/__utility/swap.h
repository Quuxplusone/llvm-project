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
#include <__type_traits/datasizeof.h>
#include <__type_traits/is_assignable.h>
#include <__type_traits/is_constant_evaluated.h>
#include <__type_traits/is_constructible.h>
#include <__type_traits/is_constant_evaluated.h>
#include <__type_traits/is_empty.h>
#include <__type_traits/is_nothrow_assignable.h>
#include <__type_traits/is_nothrow_constructible.h>
#include <__type_traits/is_swappable.h>
#include <__type_traits/is_trivially_relocatable.h>
#include <__type_traits/is_volatile.h>
#include <__utility/move.h>
#include <cstddef>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_PUSH_MACROS
#include <__undef_macros>

_LIBCPP_BEGIN_NAMESPACE_STD

#ifndef _LIBCPP_CXX03_LANG
template <class _Tp>
using __swap_result_t = __enable_if_t<is_move_constructible<_Tp>::value && is_move_assignable<_Tp>::value>;
#else
template <class>
using __swap_result_t = void;
#endif

#ifndef _LIBCPP_TRIVIAL_SWAP_SIZE
#  define _LIBCPP_TRIVIAL_SWAP_SIZE 128
#endif

inline _LIBCPP_HIDE_FROM_ABI void __libcpp_memswap(volatile void* __vpx, volatile void* __vpy, size_t __n) {
  char* __px = static_cast<char*>(const_cast<void*>(__vpx));
  char* __py = static_cast<char*>(const_cast<void*>(__vpy));
  char __t[_LIBCPP_TRIVIAL_SWAP_SIZE];
  while (__n >= _LIBCPP_TRIVIAL_SWAP_SIZE) {
    ::__builtin_memcpy(__t, __px, _LIBCPP_TRIVIAL_SWAP_SIZE);
    ::__builtin_memmove(__px, __py, _LIBCPP_TRIVIAL_SWAP_SIZE);
    ::__builtin_memcpy(__py, __t, _LIBCPP_TRIVIAL_SWAP_SIZE);
    __px += _LIBCPP_TRIVIAL_SWAP_SIZE;
    __py += _LIBCPP_TRIVIAL_SWAP_SIZE;
    __n -= _LIBCPP_TRIVIAL_SWAP_SIZE;
  }
  if (__n != 0) {
    ::__builtin_memcpy(__t, __px, __n);
    ::__builtin_memmove(__px, __py, __n);
    ::__builtin_memcpy(__py, __t, __n);
  }
}

template <class _Tp>
inline _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 void __generic_swap(_Tp& __x, _Tp& __y)
    _NOEXCEPT_(is_nothrow_move_constructible<_Tp>::value && is_nothrow_move_assignable<_Tp>::value) {
  if (__libcpp_is_trivially_relocatable<_Tp>::value && !is_volatile<_Tp>::value && is_empty<_Tp>::value) {
    // Swap nothing.
  } else if (__libcpp_is_trivially_relocatable<_Tp>::value && !is_volatile<_Tp>::value && !__libcpp_is_constant_evaluated()) {
    // Swap only "datasizeof" bytes.
    std::__libcpp_memswap(std::addressof(__x), std::addressof(__y), __datasizeof_v<_Tp>);
  } else {
    _Tp __t(std::move(__x));
    __x = std::move(__y);
    __y = std::move(__t);
  }
}

template <class _Tp>
inline _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 __swap_result_t<_Tp> swap(_Tp& __x, _Tp& __y)
    _NOEXCEPT_(is_nothrow_move_constructible<_Tp>::value && is_nothrow_move_assignable<_Tp>::value) {
  std::__generic_swap(__x, __y);
}

template <class _Tp, size_t _Np, __enable_if_t<__is_swappable_v<_Tp>, int> >
inline _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 void swap(_Tp (&__a)[_Np], _Tp (&__b)[_Np])
    _NOEXCEPT_(__is_nothrow_swappable_v<_Tp>) {
  if (__libcpp_is_trivially_relocatable<_Tp>::value && !is_volatile<_Tp>::value && !__libcpp_is_constant_evaluated()) {
    std::__libcpp_memswap(std::addressof(__a), std::addressof(__b), sizeof(__a));
  } else {
    for (size_t __i = 0; __i != _Np; ++__i) {
      swap(__a[__i], __b[__i]);
    }
  }
}

_LIBCPP_END_NAMESPACE_STD

_LIBCPP_POP_MACROS

#endif // _LIBCPP___UTILITY_SWAP_H
