//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___ALGORITHM_IS_UNIQUED_H
#define _LIBCPP___ALGORITHM_IS_UNIQUED_H

#include <__algorithm/comp.h>
#include <__algorithm/adjacent_find.h>
#include <__config>
#include <__iterator/iterator_traits.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _ForwardIterator, class _BinaryPredicate>
_LIBCPP_NODISCARD_EXT inline
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20
bool
is_uniqued(_ForwardIterator __first, _ForwardIterator __last, _BinaryPredicate __pred)
{
    return std::__adjacent_find(__first, __last, __pred) == __last;
}

template<class _ForwardIterator>
_LIBCPP_NODISCARD_EXT inline
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20
bool
is_uniqued(_ForwardIterator __first, _ForwardIterator __last)
{
    return std::__adjacent_find(__first, __last, __equal_to()) == __last;
}

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___ALGORITHM_IS_UNIQUED_H
