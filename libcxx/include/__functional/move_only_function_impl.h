//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// This header is only partially guarded on purpose. This header is an implementation detail of move_only_function.h
// and generates multiple versions of std::move_only_function

#include <__assert>
#include <__config>
#include <__functional/invoke.h>
#include <__memory/unique_ptr.h>
#include <__type_traits/is_member_function_pointer.h>
#include <__type_traits/is_trivially_copyable.h>
#include <__type_traits/remove_pointer.h>
#include <__utility/forward.h>
#include <__utility/in_place.h>
#include <__utility/move.h>
#include <__utility/swap.h>
#include <cstddef>
#include <initializer_list>
#include <new>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

#ifndef _LIBCPP_IN_MOVE_ONLY_FUNCTION_H
#  error This header should only be included from move_only_function.h
#endif

#ifndef _LIBCPP___FUNCTIONAL_MOVE_ONLY_FUNCTION_IMPL_H
#  define _LIBCPP___FUNCTIONAL_MOVE_ONLY_FUNCTION_IMPL_H

_LIBCPP_BEGIN_NAMESPACE_STD

struct __move_only_function_storage {
  static constexpr size_t __target_size = 6 * sizeof(void*);
  static constexpr size_t _Size = __target_size - (2 * sizeof(void(*)()));
  static constexpr size_t _Align = alignof(void(*)());

  void (*__call_)() = nullptr; // must be cast to _ReturnT(*)(_ArgTypes&&..., void*) before it is used
  void (*__destroy_)(void*) noexcept = nullptr;
  alignas(_Align) unsigned char __data_[_Size];

  _LIBCPP_HIDE_FROM_ABI void __reset() {
    if (__destroy_ != nullptr) {
      __destroy_(__data_);
    }
    __call_ = nullptr;
    __destroy_ = nullptr;
  }

  _LIBCPP_HIDE_FROM_ABI void swap(__move_only_function_storage& __rhs) noexcept {
    std::swap(__call_, __rhs.__call_);
    std::swap(__destroy_, __rhs.__destroy_);
    std::swap(__data_, __rhs.__data_);
  }
};

static_assert(alignof(__move_only_function_storage) == alignof(void*));
static_assert(sizeof(__move_only_function_storage) == __move_only_function_storage::__target_size);

template <class...>
class move_only_function;

template <class>
struct __is_move_only_function : false_type {};

template <class... _Ts>
struct __is_move_only_function<move_only_function<_Ts...>> : true_type {};

template <class, class>
struct __is_compatible_move_only_function : false_type {};

template <class _Rp, class... _Ap> struct __is_compatible_move_only_function<_Rp(_Ap...), move_only_function<_Rp(_Ap...)>> : true_type {};
template <class _Rp, class... _Ap> struct __is_compatible_move_only_function<_Rp(_Ap...), move_only_function<_Rp(_Ap...) &>> : true_type {};
template <class _Rp, class... _Ap> struct __is_compatible_move_only_function<_Rp(_Ap...), move_only_function<_Rp(_Ap...) &&>> : true_type {};
template <class _Rp, class... _Ap> struct __is_compatible_move_only_function<_Rp(_Ap...), move_only_function<_Rp(_Ap...) const>> : true_type {};
template <class _Rp, class... _Ap> struct __is_compatible_move_only_function<_Rp(_Ap...), move_only_function<_Rp(_Ap...) const &>> : true_type {};
template <class _Rp, class... _Ap> struct __is_compatible_move_only_function<_Rp(_Ap...), move_only_function<_Rp(_Ap...) const &&>> : true_type {};
template <class _Rp, class... _Ap> struct __is_compatible_move_only_function<_Rp(_Ap...), move_only_function<_Rp(_Ap...) noexcept>> : true_type {};
template <class _Rp, class... _Ap> struct __is_compatible_move_only_function<_Rp(_Ap...), move_only_function<_Rp(_Ap...) & noexcept>> : true_type {};
template <class _Rp, class... _Ap> struct __is_compatible_move_only_function<_Rp(_Ap...), move_only_function<_Rp(_Ap...) && noexcept>> : true_type {};
template <class _Rp, class... _Ap> struct __is_compatible_move_only_function<_Rp(_Ap...), move_only_function<_Rp(_Ap...) const noexcept>> : true_type {};
template <class _Rp, class... _Ap> struct __is_compatible_move_only_function<_Rp(_Ap...), move_only_function<_Rp(_Ap...) const & noexcept>> : true_type {};
template <class _Rp, class... _Ap> struct __is_compatible_move_only_function<_Rp(_Ap...), move_only_function<_Rp(_Ap...) const && noexcept>> : true_type {};

_LIBCPP_END_NAMESPACE_STD

// This header is only partially guarded on purpose. This header is an implementation detail of move_only_function.h
// and generates multiple versions of std::move_only_function
#endif // _LIBCPP___FUNCTIONAL_MOVE_ONLY_FUNCTION_IMPL_H

#ifndef _LIBCPP_MOVE_ONLY_FUNCTION_CV
#  define _LIBCPP_MOVE_ONLY_FUNCTION_CV
#endif

#ifndef _LIBCPP_MOVE_ONLY_FUNCTION_REF
#  define _LIBCPP_MOVE_ONLY_FUNCTION_REF
#  define _LIBCPP_MOVE_ONLY_FUNCTION_INV_QUALS _LIBCPP_MOVE_ONLY_FUNCTION_CV&
#else
#  define _LIBCPP_MOVE_ONLY_FUNCTION_INV_QUALS _LIBCPP_MOVE_ONLY_FUNCTION_CV _LIBCPP_MOVE_ONLY_FUNCTION_REF
#endif

#ifndef _LIBCPP_MOVE_ONLY_FUNCTION_NOEXCEPT
#  define _LIBCPP_MOVE_ONLY_FUNCTION_NOEXCEPT false
#endif

#define _LIBCPP_MOVE_ONLY_FUNCTION_CV_REF _LIBCPP_MOVE_ONLY_FUNCTION_CV _LIBCPP_MOVE_ONLY_FUNCTION_REF

_LIBCPP_BEGIN_NAMESPACE_STD

#ifdef _LIBCPP_ABI_MOVE_ONLY_FUNCTION_TRIVIAL_ABI
#  define _LIBCPP_MOVE_ONLY_FUNCTION_TRIVIAL_ABI [[_Clang::__trivial_abi__]]
#else
#  define _LIBCPP_MOVE_ONLY_FUNCTION_TRIVIAL_ABI
#endif

template <class _ReturnT, class... _ArgTypes>
class
  _LIBCPP_MOVE_ONLY_FUNCTION_TRIVIAL_ABI
  move_only_function<_ReturnT(_ArgTypes...) _LIBCPP_MOVE_ONLY_FUNCTION_CV_REF noexcept(_LIBCPP_MOVE_ONLY_FUNCTION_NOEXCEPT)>
{
  template <class...>
  friend class move_only_function; // for access to __storage_

  template <class _VT>
  static constexpr bool __is_callable_from_impl() {
    if (_LIBCPP_MOVE_ONLY_FUNCTION_NOEXCEPT) {
      return is_nothrow_invocable_r_v<_ReturnT, _VT _LIBCPP_MOVE_ONLY_FUNCTION_CV_REF, _ArgTypes...> &&
             is_nothrow_invocable_r_v<_ReturnT, _VT _LIBCPP_MOVE_ONLY_FUNCTION_INV_QUALS, _ArgTypes...>;
    } else {
      return is_invocable_r_v<_ReturnT, _VT _LIBCPP_MOVE_ONLY_FUNCTION_CV_REF, _ArgTypes...> &&
             is_invocable_r_v<_ReturnT, _VT _LIBCPP_MOVE_ONLY_FUNCTION_INV_QUALS, _ArgTypes...>;
    }
    return false;
  }

  template <class _VT>
  static constexpr bool __is_callable_from = __is_callable_from_impl<decay_t<_VT>>();

  template <class _StoredFunc, class... _Args>
  _LIBCPP_HIDE_FROM_ABI void __construct(_Args&&... __args) {
    static_assert(is_same_v<decay_t<_StoredFunc>, _StoredFunc>);
    static_assert(is_constructible_v<_StoredFunc, _StoredFunc&&>);

    constexpr bool __fits_in_buffer =
      is_trivially_copyable_v<_StoredFunc> &&
      sizeof(_StoredFunc) <= __move_only_function_storage::_Size &&
      alignof(_StoredFunc) <= __move_only_function_storage::_Align;

    if constexpr (__fits_in_buffer) {
      __storage_.__call_ = reinterpret_cast<void(*)()>(
        +[](_ArgTypes&&... __argts, void *__data) noexcept(_LIBCPP_MOVE_ONLY_FUNCTION_NOEXCEPT) -> _ReturnT {
          _StoredFunc *__p = reinterpret_cast<_StoredFunc*>(__data);
          return std::invoke_r<_ReturnT>(static_cast<_StoredFunc _LIBCPP_MOVE_ONLY_FUNCTION_INV_QUALS>(*__p),
                                         std::forward<_ArgTypes>(__argts)...);
        }
      );
      __storage_.__destroy_ = +[](void *__data) noexcept {
        _StoredFunc *__p = reinterpret_cast<_StoredFunc*>(__data);
        __p->~_StoredFunc();
      };
      ::new ((void*)__storage_.__data_) _StoredFunc(std::forward<_Args>(__args)...);
    } else {
      using _StoredFuncPtr = _StoredFunc*;
      __storage_.__call_ = reinterpret_cast<void(*)()>(
        +[](_ArgTypes&&... __argts, void *__data) noexcept(_LIBCPP_MOVE_ONLY_FUNCTION_NOEXCEPT) -> _ReturnT {
          _StoredFunc *__p = *reinterpret_cast<_StoredFunc**>(__data);
          return std::invoke_r<_ReturnT>(static_cast<_StoredFunc _LIBCPP_MOVE_ONLY_FUNCTION_INV_QUALS>(*__p),
                                         std::forward<_ArgTypes>(__argts)...);
        }
      );
      __storage_.__destroy_ = +[](void *__data) noexcept {
        _StoredFunc *__p = *reinterpret_cast<_StoredFunc**>(__data);
        __p->~_StoredFunc();
        ::operator delete(static_cast<void *>(__p));
      };
      unique_ptr<_StoredFunc> __u = std::make_unique<_StoredFunc>(std::forward<_Args>(__args)...);
      ::new ((void*)__storage_.__data_) _StoredFuncPtr(__u.release());
    }
  }

public:
  using result_type = _ReturnT;

  // [func.wrap.move.ctor]
  move_only_function() noexcept = default;
  _LIBCPP_HIDE_FROM_ABI move_only_function(nullptr_t) noexcept {}
  _LIBCPP_HIDE_FROM_ABI move_only_function(move_only_function&& __other) noexcept : __storage_(__other.__storage_) {
    __other.__storage_ = {};
  }

  template <class _Func>
    requires(!is_same_v<__remove_cvref_t<_Func>, move_only_function> && !__is_inplace_type<__remove_cvref_t<_Func>>::value &&
             __is_callable_from<_Func>)
  _LIBCPP_HIDE_FROM_ABI move_only_function(_Func&& __func) noexcept {
    using _StoredFunc = decay_t<_Func>;

    if constexpr ((is_pointer_v<_StoredFunc> && is_function_v<remove_pointer_t<_StoredFunc>>) ||
                  is_member_function_pointer_v<_StoredFunc>) {
      if (__func == nullptr) {
        // we're already disengaged
      } else {
        __construct<_StoredFunc>(std::forward<_Func>(__func));
      }
    } else if constexpr (__is_move_only_function<_StoredFunc>::value) {
      if constexpr (__is_compatible_move_only_function<_ReturnT(_ArgTypes...), _StoredFunc>::value) {
        __storage_ = __func.__storage_;
        __func.__storage_ = {};
      } else if (!__func) {
        // we're already disengaged
      } else {
        __construct<_StoredFunc>(std::forward<_Func>(__func));
      }
    } else {
      __construct<_StoredFunc>(std::forward<_Func>(__func));
    }
  }

  template <class _Func, class... _Args>
    requires is_constructible_v<decay_t<_Func>, _Args...> && __is_callable_from<_Func>
  _LIBCPP_HIDE_FROM_ABI explicit move_only_function(in_place_type_t<_Func>, _Args&&... __args) {
    static_assert(is_same_v<decay_t<_Func>, _Func>);
    __construct<_Func>(std::forward<_Args>(__args)...);
  }

  template <class _Func, class _InitListType, class... _Args>
    requires is_constructible_v<decay_t<_Func>, initializer_list<_InitListType>&, _Args...> && __is_callable_from<_Func>
  _LIBCPP_HIDE_FROM_ABI explicit move_only_function(
      in_place_type_t<_Func>, initializer_list<_InitListType> __il, _Args&&... __args) {
    static_assert(is_same_v<decay_t<_Func>, _Func>);
    __construct<_Func>(__il, std::forward<_Args>(__args)...);
  }

  _LIBCPP_HIDE_FROM_ABI move_only_function& operator=(move_only_function&& __other) noexcept {
    move_only_function(std::move(__other)).swap(*this);
    return *this;
  }

  _LIBCPP_HIDE_FROM_ABI move_only_function& operator=(nullptr_t) noexcept {
    __storage_.__reset();
    return *this;
  }

  template <class _Func>
    requires (!is_same_v<__remove_cvref_t<_Func>, move_only_function> &&
              !__is_inplace_type<__remove_cvref_t<_Func>>::value &&
              __is_callable_from<_Func>)
  _LIBCPP_HIDE_FROM_ABI move_only_function& operator=(_Func&& __func) noexcept {
    move_only_function(std::forward<_Func>(__func)).swap(*this);
  }

  _LIBCPP_HIDE_FROM_ABI ~move_only_function() { __storage_.__reset(); }

  // [func.wrap.move.inv]
  _LIBCPP_HIDE_FROM_ABI explicit operator bool() const noexcept {
    return __storage_.__call_ != nullptr;
  }

  _LIBCPP_HIDE_FROM_ABI _ReturnT operator()(_ArgTypes... __argts) _LIBCPP_MOVE_ONLY_FUNCTION_CV_REF
      noexcept(_LIBCPP_MOVE_ONLY_FUNCTION_NOEXCEPT) {
    _LIBCPP_ASSERT_VALID_ELEMENT_ACCESS(__storage_.__call_ != nullptr, "Tried to call a disengaged move_only_function");

    using _CallType = _ReturnT(*)(_ArgTypes&&..., void*) noexcept(_LIBCPP_MOVE_ONLY_FUNCTION_NOEXCEPT);
    _CallType __pcall = reinterpret_cast<_CallType>(__storage_.__call_);
    return __pcall(std::forward<_ArgTypes>(__argts)..., (void*)__storage_.__data_);
  }

  // [func.wrap.move.util]
  _LIBCPP_HIDE_FROM_ABI void swap(move_only_function& __other) noexcept {
    __storage_.swap(__other.__storage_);
  }

  _LIBCPP_HIDE_FROM_ABI friend void swap(move_only_function& __lhs, move_only_function& __rhs) noexcept {
    __lhs.swap(__rhs);
  }

  _LIBCPP_HIDE_FROM_ABI friend bool operator==(const move_only_function& __func, nullptr_t) noexcept { return !__func; }

private:
  __move_only_function_storage __storage_;
};

#undef _LIBCPP_MOVE_ONLY_FUNCTION_CV
#undef _LIBCPP_MOVE_ONLY_FUNCTION_REF
#undef _LIBCPP_MOVE_ONLY_FUNCTION_NOEXCEPT
#undef _LIBCPP_MOVE_ONLY_FUNCTION_INV_QUALS
#undef _LIBCPP_MOVE_ONLY_FUNCTION_CV_REF

_LIBCPP_END_NAMESPACE_STD
