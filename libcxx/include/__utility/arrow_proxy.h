//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___UTILITY_ARROW_PROXY_H
#define _LIBCPP___UTILITY_ARROW_PROXY_H

#include <__config>
#include <__memory/addressof.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

template<class _Reference>
class __arrow_proxy {
public:
  explicit __arrow_proxy(_Reference __r) : __r_(__r) {}
  const _Reference *operator->() const { return std::addressof(__r_); }
private:
  _Reference __r_;
};

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___UTILITY_ARROW_PROXY_H
