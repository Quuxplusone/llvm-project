//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___EXCEPTION_EXCEPTION_PTR_H
#define _LIBCPP___EXCEPTION_EXCEPTION_PTR_H

#include <__config>
#include <__exception/operations.h>
#include <__memory/addressof.h>
#include <__type_traits/is_array.h>
#include <__type_traits/is_object.h>
#include <__type_traits/is_same.h>
#include <__type_traits/remove_cvref.h>
#include <cstddef>
#include <cstdlib>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

namespace std { // purposefully not using versioning namespace

#ifndef _LIBCPP_ABI_MICROSOFT

class _LIBCPP_EXPORTED_FROM_ABI _LIBCPP_TRIVIALLY_RELOCATABLE exception_ptr {
  void* __ptr_;

public:
  _LIBCPP_HIDE_FROM_ABI exception_ptr() _NOEXCEPT : __ptr_() {}
  _LIBCPP_HIDE_FROM_ABI exception_ptr(nullptr_t) _NOEXCEPT : __ptr_() {}

  exception_ptr(const exception_ptr&) _NOEXCEPT;
  exception_ptr& operator=(const exception_ptr&) _NOEXCEPT;
  ~exception_ptr() _NOEXCEPT;

  _LIBCPP_HIDE_FROM_ABI explicit operator bool() const _NOEXCEPT { return __ptr_ != nullptr; }

#if _LIBCPP_STD_VER >= 20
  // This makes exception_ptr trivially equality comparable
  friend _LIBCPP_HIDE_FROM_ABI bool operator==(const exception_ptr&, const exception_ptr&) = default;
#else
  friend _LIBCPP_HIDE_FROM_ABI bool operator==(const exception_ptr& __x, const exception_ptr& __y) _NOEXCEPT {
    return __x.__ptr_ == __y.__ptr_;
  }

  friend _LIBCPP_HIDE_FROM_ABI bool operator!=(const exception_ptr& __x, const exception_ptr& __y) _NOEXCEPT {
    return !(__x == __y);
  }
#endif

  friend _LIBCPP_EXPORTED_FROM_ABI exception_ptr current_exception() _NOEXCEPT;
  friend _LIBCPP_EXPORTED_FROM_ABI void rethrow_exception(exception_ptr);
};

template <class _Ep>
_LIBCPP_HIDE_FROM_ABI exception_ptr make_exception_ptr(_Ep __e) _NOEXCEPT {
#  ifndef _LIBCPP_HAS_NO_EXCEPTIONS
  try {
    throw __e;
  } catch (...) {
    return current_exception();
  }
#  else
  ((void)__e);
  std::abort();
#  endif
}

#else // _LIBCPP_ABI_MICROSOFT

class _LIBCPP_EXPORTED_FROM_ABI exception_ptr {
  _LIBCPP_DIAGNOSTIC_PUSH
  _LIBCPP_CLANG_DIAGNOSTIC_IGNORED("-Wunused-private-field")
  void* __ptr1_;
  void* __ptr2_;
  _LIBCPP_DIAGNOSTIC_POP

public:
  exception_ptr() _NOEXCEPT;
  exception_ptr(nullptr_t) _NOEXCEPT;
  exception_ptr(const exception_ptr& __other) _NOEXCEPT;
  exception_ptr& operator=(const exception_ptr& __other) _NOEXCEPT;
  exception_ptr& operator=(nullptr_t) _NOEXCEPT;
  ~exception_ptr() _NOEXCEPT;
  explicit operator bool() const _NOEXCEPT;
};

_LIBCPP_EXPORTED_FROM_ABI bool operator==(const exception_ptr& __x, const exception_ptr& __y) _NOEXCEPT;

inline _LIBCPP_HIDE_FROM_ABI bool operator!=(const exception_ptr& __x, const exception_ptr& __y) _NOEXCEPT {
  return !(__x == __y);
}

_LIBCPP_EXPORTED_FROM_ABI void swap(exception_ptr&, exception_ptr&) _NOEXCEPT;

_LIBCPP_EXPORTED_FROM_ABI exception_ptr __copy_exception_ptr(void* __except, const void* __ptr);
_LIBCPP_EXPORTED_FROM_ABI exception_ptr current_exception() _NOEXCEPT;
_LIBCPP_NORETURN _LIBCPP_EXPORTED_FROM_ABI void rethrow_exception(exception_ptr);

// This is a built-in template function which automagically extracts the required
// information.
template <class _E>
void* __GetExceptionInfo(_E);

template <class _Ep>
_LIBCPP_HIDE_FROM_ABI exception_ptr make_exception_ptr(_Ep __e) _NOEXCEPT {
  return __copy_exception_ptr(std::addressof(__e), __GetExceptionInfo(__e));
}

#endif // _LIBCPP_ABI_MICROSOFT

template <class _Ep>
_LIBCPP_HIDE_FROM_ABI const __remove_cvref_t<_Ep>* try_cast(const exception_ptr& __p) _NOEXCEPT {
  static_assert(is_object<_Ep>::value && is_same<__remove_cvref_t<_Ep>, _Ep>::value &&
                !is_array<_Ep>::value && sizeof(_Ep) >= 0,
                "E must be a complete cv-unqualified object type, and not an array type");
  // E must also not be a pointer to an incomplete type; this is caught by the core language below.
  if (__p == nullptr) {
    return nullptr;
  }
#  ifndef _LIBCPP_HAS_NO_EXCEPTIONS
  try {
    std::rethrow_exception(__p);
  } catch (const _Ep& __ex) {
    try {
      std::rethrow_exception(__p);
    } catch (const _Ep& __also_ex) {
      _LIBCPP_ASSERT(&__ex == &__also_ex, "try_cast must be specially implemented on platforms where rethrowing copies the exception");
    }
    return std::addressof(__ex);
  } catch (...) {
    return nullptr;
  }
#  else
  ((void)__p);
  std::abort();
#  endif
}

} // namespace std

#endif // _LIBCPP___EXCEPTION_EXCEPTION_PTR_H
