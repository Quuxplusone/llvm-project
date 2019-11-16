// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___MEMORY_UNINITIALIZED_RELOCATE_H
#define _LIBCPP___MEMORY_UNINITIALIZED_RELOCATE_H

#include <__config>
#include <__iterator/iterator_traits.h>
#include <__memory/addressof.h>
#include <__memory/construct_at.h>
#include <__memory/pointer_traits.h>
#include <__memory/relocate_at.h>
#include <__type_traits/integral_constant.h>
#include <__type_traits/is_nothrow_constructible.h>
#include <__type_traits/is_trivially_relocatable.h>
#include <__type_traits/is_volatile.h>
#include <__type_traits/remove_cvref.h>
#include <__utility/move.h>
#include <__utility/pair.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

template<class _InputIt, class _ForwardIt>
inline _LIBCPP_INLINE_VISIBILITY
_ForwardIt
__uninitialized_relocate_impl(_InputIt __first, _InputIt __last,
                              _ForwardIt __result, false_type, false_type)
{
    _ForwardIt __orig_result = __result;
    for (; __first != __last; ++__result, void(), ++__first) {
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
#endif
        std::__libcpp_relocate_at2(0, std::addressof(*__first), std::addressof(*__result));
#ifndef _LIBCPP_NO_EXCEPTIONS
        } catch (...) {
            std::__destroy(__orig_result, __result);
            ++__first;
            std::__destroy(__first, __last);
            throw;
        }
#endif
    }
    return __result;
}

template<class _InputIt, class _ForwardIt>
inline _LIBCPP_INLINE_VISIBILITY
_ForwardIt
__uninitialized_relocate_impl(_InputIt __first, _InputIt __last,
                              _ForwardIt __result, false_type, true_type)
{
    for (; __first != __last; ++__result, void(), ++__first) {
        std::__libcpp_relocate_at2(0, std::addressof(*__first), std::addressof(*__result));
    }
    return __result;
}

template<class _InputIt, class _ForwardIt, class _LoopWithoutCatchIgnored>
inline _LIBCPP_INLINE_VISIBILITY
_ForwardIt
__uninitialized_relocate_impl(_InputIt __first, _InputIt __last,
                              _ForwardIt __result, true_type, _LoopWithoutCatchIgnored)
{
    auto __count = __last - __first;
    if (__count != 0) {
        char *__firstbyte = (char *)std::__to_address(__first);
        auto __nbytes = (char *)std::__to_address(__last) - __firstbyte;
        ::__builtin_memmove(std::__to_address(__result), __firstbyte, __nbytes);
        __result += __count;
    }
    return __result;
}

template<class _InputIt, class _ForwardIt>
inline _LIBCPP_INLINE_VISIBILITY
_ForwardIt
__libcpp_uninitialized_relocate(_InputIt __first, _InputIt __last,
                                _ForwardIt __result)
{
    using _St = decltype(std::move(*__first));
    using _Dt = typename iterator_traits<_ForwardIt>::value_type;
    using _ElementTypeIsMemcpyable = integral_constant<bool,
        __is_same_uncvref<_St, _Dt>::value &&
        __libcpp_is_trivially_relocatable<_Dt>::value &&
        !is_volatile<_St>::value && !is_volatile<_Dt>::value
    >;
    using _SingleMemcpy = integral_constant<bool,
        _ElementTypeIsMemcpyable::value &&
        __is_cpp17_contiguous_iterator<_InputIt>::value &&
        __is_cpp17_contiguous_iterator<_ForwardIt>::value
    >;
    using _LoopWithoutCatch = is_nothrow_constructible<_Dt, _St>;
    return std::__uninitialized_relocate_impl(__first, __last, __result,
        _SingleMemcpy(), _LoopWithoutCatch());
}

// __libcpp_uninitialized_relocate_n

template<class _InputIt, class _Size, class _ForwardIt>
inline _LIBCPP_INLINE_VISIBILITY
pair<_InputIt, _ForwardIt>
__uninitialized_relocate_n_impl(_InputIt __first, _Size __n,
                                _ForwardIt __result, false_type, false_type)
{
    _ForwardIt __orig_result = __result;
    for (; __n > 0; ++__result, void(), ++__first, void(), --__n) {
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
#endif
            std::__libcpp_relocate_at2(0, std::addressof(*__first), std::addressof(*__result));
#ifndef _LIBCPP_NO_EXCEPTIONS
        } catch (...) {
            std::__destroy(__orig_result, __result);
            for (++__first, void(), --__n; __n > 0; ++__first, void(), --__n) {
                std::__destroy_at(std::addressof(*__first));
            }
            throw;
        }
#endif
    }
    return {__first, __result};
}

template<class _InputIt, class _Size, class _ForwardIt>
inline _LIBCPP_INLINE_VISIBILITY
pair<_InputIt, _ForwardIt>
__uninitialized_relocate_n_impl(_InputIt __first, _Size __n,
                                _ForwardIt __result, false_type, true_type)
{
    for (; __n > 0; ++__result, ++__first, void(), --__n) {
        std::__libcpp_relocate_at2(0, std::addressof(*__first), std::addressof(*__result));
    }
    return {__first, __result};
}

template<class _InputIt, class _Size, class _ForwardIt, class _LoopWithoutCatchIgnored>
inline _LIBCPP_INLINE_VISIBILITY
pair<_InputIt, _ForwardIt>
__uninitialized_relocate_n_impl(_InputIt __first, _Size __n,
                                _ForwardIt __result, true_type, _LoopWithoutCatchIgnored)
{
    using _Dt = typename iterator_traits<_ForwardIt>::value_type;
    if (__n != 0) {
        ::__builtin_memmove(std::addressof(*__result), std::addressof(*__first), __n * sizeof (_Dt));
        __first += __n;
        __result += __n;
    }
    return {__first, __result};
}

template<class _InputIt, class _Size, class _ForwardIt>
inline _LIBCPP_INLINE_VISIBILITY
pair<_InputIt, _ForwardIt>
__libcpp_uninitialized_relocate_n(_InputIt __first, _Size __n, _ForwardIt __result)
{
    using _St = decltype(std::move(*__first));
    using _Dt = typename iterator_traits<_ForwardIt>::value_type;
    using _ElementTypeIsMemcpyable = integral_constant<bool,
        __is_same_uncvref<_St, _Dt>::value &&
        __libcpp_is_trivially_relocatable<_Dt>::value &&
        !is_volatile<_St>::value && !is_volatile<_Dt>::value
    >;
    using _SingleMemcpy = integral_constant<bool,
        _ElementTypeIsMemcpyable::value &&
        __is_cpp17_contiguous_iterator<_InputIt>::value &&
        __is_cpp17_contiguous_iterator<_ForwardIt>::value
    >;
    using _LoopWithoutCatch = is_nothrow_constructible<_Dt, _St>;
    return std::__uninitialized_relocate_n_impl(__first, __n, __result,
        _SingleMemcpy(), _LoopWithoutCatch());
}

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___MEMORY_UNINITIALIZED_RELOCATE_H
