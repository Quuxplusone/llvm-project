// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___MEMORY_UNINITIALIZED_ALGORITHMS_H
#define _LIBCPP___MEMORY_UNINITIALIZED_ALGORITHMS_H

#include <__config>
#include <__memory/addressof.h>
#include <__memory/construct_at.h>
#include <__memory/relocate_at.h>
#include <cstring>
#include <iterator>
#include <utility>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _InputIterator, class _ForwardIterator>
_ForwardIterator
uninitialized_copy(_InputIterator __f, _InputIterator __l, _ForwardIterator __r)
{
    typedef typename iterator_traits<_ForwardIterator>::value_type value_type;
#ifndef _LIBCPP_NO_EXCEPTIONS
    _ForwardIterator __s = __r;
    try
    {
#endif
        for (; __f != __l; ++__f, (void) ++__r)
            ::new ((void*)_VSTD::addressof(*__r)) value_type(*__f);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        for (; __s != __r; ++__s)
            __s->~value_type();
        throw;
    }
#endif
    return __r;
}

template <class _InputIterator, class _Size, class _ForwardIterator>
_ForwardIterator
uninitialized_copy_n(_InputIterator __f, _Size __n, _ForwardIterator __r)
{
    typedef typename iterator_traits<_ForwardIterator>::value_type value_type;
#ifndef _LIBCPP_NO_EXCEPTIONS
    _ForwardIterator __s = __r;
    try
    {
#endif
        for (; __n > 0; ++__f, (void) ++__r, (void) --__n)
            ::new ((void*)_VSTD::addressof(*__r)) value_type(*__f);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        for (; __s != __r; ++__s)
            __s->~value_type();
        throw;
    }
#endif
    return __r;
}

template <class _ForwardIterator, class _Tp>
void
uninitialized_fill(_ForwardIterator __f, _ForwardIterator __l, const _Tp& __x)
{
    typedef typename iterator_traits<_ForwardIterator>::value_type value_type;
#ifndef _LIBCPP_NO_EXCEPTIONS
    _ForwardIterator __s = __f;
    try
    {
#endif
        for (; __f != __l; ++__f)
            ::new ((void*)_VSTD::addressof(*__f)) value_type(__x);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        for (; __s != __f; ++__s)
            __s->~value_type();
        throw;
    }
#endif
}

template <class _ForwardIterator, class _Size, class _Tp>
_ForwardIterator
uninitialized_fill_n(_ForwardIterator __f, _Size __n, const _Tp& __x)
{
    typedef typename iterator_traits<_ForwardIterator>::value_type value_type;
#ifndef _LIBCPP_NO_EXCEPTIONS
    _ForwardIterator __s = __f;
    try
    {
#endif
        for (; __n > 0; ++__f, (void) --__n)
            ::new ((void*)_VSTD::addressof(*__f)) value_type(__x);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        for (; __s != __f; ++__s)
            __s->~value_type();
        throw;
    }
#endif
    return __f;
}

#if _LIBCPP_STD_VER > 14

template <class _ForwardIterator>
inline _LIBCPP_INLINE_VISIBILITY
void uninitialized_default_construct(_ForwardIterator __first, _ForwardIterator __last) {
    using _Vt = typename iterator_traits<_ForwardIterator>::value_type;
    auto __idx = __first;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
#endif
    for (; __idx != __last; ++__idx)
        ::new ((void*)_VSTD::addressof(*__idx)) _Vt;
#ifndef _LIBCPP_NO_EXCEPTIONS
    } catch (...) {
        _VSTD::destroy(__first, __idx);
        throw;
    }
#endif
}

template <class _ForwardIterator, class _Size>
inline _LIBCPP_INLINE_VISIBILITY
_ForwardIterator uninitialized_default_construct_n(_ForwardIterator __first, _Size __n) {
    using _Vt = typename iterator_traits<_ForwardIterator>::value_type;
    auto __idx = __first;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
#endif
    for (; __n > 0; ++__idx, (void) --__n)
        ::new ((void*)_VSTD::addressof(*__idx)) _Vt;
    return __idx;
#ifndef _LIBCPP_NO_EXCEPTIONS
    } catch (...) {
        _VSTD::destroy(__first, __idx);
        throw;
    }
#endif
}


template <class _ForwardIterator>
inline _LIBCPP_INLINE_VISIBILITY
void uninitialized_value_construct(_ForwardIterator __first, _ForwardIterator __last) {
    using _Vt = typename iterator_traits<_ForwardIterator>::value_type;
    auto __idx = __first;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
#endif
    for (; __idx != __last; ++__idx)
        ::new ((void*)_VSTD::addressof(*__idx)) _Vt();
#ifndef _LIBCPP_NO_EXCEPTIONS
    } catch (...) {
        _VSTD::destroy(__first, __idx);
        throw;
    }
#endif
}

template <class _ForwardIterator, class _Size>
inline _LIBCPP_INLINE_VISIBILITY
_ForwardIterator uninitialized_value_construct_n(_ForwardIterator __first, _Size __n) {
    using _Vt = typename iterator_traits<_ForwardIterator>::value_type;
    auto __idx = __first;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
#endif
    for (; __n > 0; ++__idx, (void) --__n)
        ::new ((void*)_VSTD::addressof(*__idx)) _Vt();
    return __idx;
#ifndef _LIBCPP_NO_EXCEPTIONS
    } catch (...) {
        _VSTD::destroy(__first, __idx);
        throw;
    }
#endif
}


template <class _InputIt, class _ForwardIt>
inline _LIBCPP_INLINE_VISIBILITY
_ForwardIt uninitialized_move(_InputIt __first, _InputIt __last, _ForwardIt __first_res) {
    using _Vt = typename iterator_traits<_ForwardIt>::value_type;
    auto __idx = __first_res;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
#endif
    for (; __first != __last; ++__idx, (void) ++__first)
        ::new ((void*)_VSTD::addressof(*__idx)) _Vt(_VSTD::move(*__first));
    return __idx;
#ifndef _LIBCPP_NO_EXCEPTIONS
    } catch (...) {
        _VSTD::destroy(__first_res, __idx);
        throw;
    }
#endif
}

template <class _InputIt, class _Size, class _ForwardIt>
inline _LIBCPP_INLINE_VISIBILITY
pair<_InputIt, _ForwardIt>
uninitialized_move_n(_InputIt __first, _Size __n, _ForwardIt __first_res) {
    using _Vt = typename iterator_traits<_ForwardIt>::value_type;
    auto __idx = __first_res;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
#endif
    for (; __n > 0; ++__idx, (void) ++__first, --__n)
        ::new ((void*)_VSTD::addressof(*__idx)) _Vt(_VSTD::move(*__first));
    return {__first, __idx};
#ifndef _LIBCPP_NO_EXCEPTIONS
    } catch (...) {
        _VSTD::destroy(__first_res, __idx);
        throw;
    }
#endif
}

// __libcpp_uninitialized_relocate

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
        _VSTD::__libcpp_relocate_at2(0, _VSTD::addressof(*__first), _VSTD::addressof(*__result));
#ifndef _LIBCPP_NO_EXCEPTIONS
        } catch (...) {
            _VSTD::destroy(__orig_result, __result);
            ++__first;
            _VSTD::destroy(__first, __last);
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
        _VSTD::__libcpp_relocate_at2(0, _VSTD::addressof(*__first), _VSTD::addressof(*__result));
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
        char *__firstbyte = (char *)_VSTD::__to_address(__first);
        size_t __nbytes = (char *)_VSTD::__to_address(__last) - __firstbyte;
        _VSTD::memmove(_VSTD::__to_address(__result), __firstbyte, __nbytes);
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
    using _St = decltype(_VSTD::move(*__first));
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
    return _VSTD::__uninitialized_relocate_impl(__first, __last, __result,
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
        _VSTD::__libcpp_relocate_at2(0, _VSTD::addressof(*__first), _VSTD::addressof(*__result));
#ifndef _LIBCPP_NO_EXCEPTIONS
        } catch (...) {
            _VSTD::destroy(__orig_result, __result);
            for (++__first, void(), --__n; __n > 0; ++__first, void(), --__n) {
                _VSTD::destroy_at(_VSTD::addressof(*__first));
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
        _VSTD::__libcpp_relocate_at2(0, _VSTD::addressof(*__first), _VSTD::addressof(*__result));
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
        _VSTD::memmove(_VSTD::addressof(*__result), _VSTD::addressof(*__first), __n * sizeof (_Dt));
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
    using _St = decltype(_VSTD::move(*__first));
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
    return _VSTD::__uninitialized_relocate_n_impl(__first, __n, __result,
        _SingleMemcpy(), _LoopWithoutCatch());
}

#endif // _LIBCPP_STD_VER > 14

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___MEMORY_UNINITIALIZED_ALGORITHMS_H
