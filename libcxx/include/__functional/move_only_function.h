//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___FUNCTIONAL_MOVE_ONLY_FUNCTION_H
#define _LIBCPP___FUNCTIONAL_MOVE_ONLY_FUNCTION_H

#include <__config>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_PUSH_MACROS
#include <__undef_macros>

#if _LIBCPP_STD_VER >= 23

// NOLINTBEGIN(readability-duplicate-include)
#  define _LIBCPP_IN_MOVE_ONLY_FUNCTION_H

#  include <__functional/move_only_function_impl.h>

#  define _LIBCPP_MOVE_ONLY_FUNCTION_REF &
#  include <__functional/move_only_function_impl.h>

#  define _LIBCPP_MOVE_ONLY_FUNCTION_REF &&
#  include <__functional/move_only_function_impl.h>

#  define _LIBCPP_MOVE_ONLY_FUNCTION_CV const
#  include <__functional/move_only_function_impl.h>

#  define _LIBCPP_MOVE_ONLY_FUNCTION_CV const
#  define _LIBCPP_MOVE_ONLY_FUNCTION_REF &
#  include <__functional/move_only_function_impl.h>

#  define _LIBCPP_MOVE_ONLY_FUNCTION_CV const
#  define _LIBCPP_MOVE_ONLY_FUNCTION_REF &&
#  include <__functional/move_only_function_impl.h>

#  define _LIBCPP_MOVE_ONLY_FUNCTION_NOEXCEPT true
#  include <__functional/move_only_function_impl.h>

#  define _LIBCPP_MOVE_ONLY_FUNCTION_NOEXCEPT true
#  define _LIBCPP_MOVE_ONLY_FUNCTION_REF &
#  include <__functional/move_only_function_impl.h>

#  define _LIBCPP_MOVE_ONLY_FUNCTION_NOEXCEPT true
#  define _LIBCPP_MOVE_ONLY_FUNCTION_REF &&
#  include <__functional/move_only_function_impl.h>

#  define _LIBCPP_MOVE_ONLY_FUNCTION_NOEXCEPT true
#  define _LIBCPP_MOVE_ONLY_FUNCTION_CV const
#  include <__functional/move_only_function_impl.h>

#  define _LIBCPP_MOVE_ONLY_FUNCTION_NOEXCEPT true
#  define _LIBCPP_MOVE_ONLY_FUNCTION_CV const
#  define _LIBCPP_MOVE_ONLY_FUNCTION_REF &
#  include <__functional/move_only_function_impl.h>

#  define _LIBCPP_MOVE_ONLY_FUNCTION_NOEXCEPT true
#  define _LIBCPP_MOVE_ONLY_FUNCTION_CV const
#  define _LIBCPP_MOVE_ONLY_FUNCTION_REF &&
#  include <__functional/move_only_function_impl.h>

#  undef _LIBCPP_IN_MOVE_ONLY_FUNCTION_H
// NOLINTEND(readability-duplicate-include)

#endif // _LIBCPP_STD_VER >= 23

_LIBCPP_POP_MACROS

#endif // _LIBCPP___FUNCTIONAL_MOVE_ONLY_FUNCTION_H
