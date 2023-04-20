// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___FLAT_MAP_FLAT_MAP_ITERATOR_H
#define _LIBCPP___FLAT_MAP_FLAT_MAP_ITERATOR_H

#include <__cstddef/ptrdiff_t.h>
#include <__iterator/iterator_traits.h>
#include <__iterator/iter_move.h>
#include <__iterator/product_iterator.h>
#include <__type_traits/is_convertible.h>
#include <__utility/arrow_proxy.h>
#include <__utility/move.h>
#include <__utility/pair.h>
#include <compare>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_PUSH_MACROS
#include <__undef_macros>

#if _LIBCPP_STD_VER >= 23

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _KeyIt, class _MappedIt>
class __flat_map_iterator {
public:
  using value_type = pair<iter_value_t<_KeyIt>, iter_value_t<_MappedIt>>;
  using reference = pair<iter_reference_t<_KeyIt>, iter_reference_t<_MappedIt>>;
  using pointer = __arrow_proxy<reference>;
  using difference_type = ptrdiff_t;
  using iterator_category = random_access_iterator_tag;
  using __rvalue_reference = pair<iter_rvalue_reference_t<_KeyIt>, iter_rvalue_reference_t<_MappedIt>>;

  _LIBCPP_HIDE_FROM_ABI explicit __flat_map_iterator() = default;
  _LIBCPP_HIDE_FROM_ABI __flat_map_iterator(const __flat_map_iterator&) = default;
  _LIBCPP_HIDE_FROM_ABI __flat_map_iterator(__flat_map_iterator&&) = default;
  _LIBCPP_HIDE_FROM_ABI __flat_map_iterator& operator=(const __flat_map_iterator&) = default;
  _LIBCPP_HIDE_FROM_ABI __flat_map_iterator& operator=(__flat_map_iterator&&) = default;

  template <class _K2, class _M2>
    requires std::is_convertible_v<_K2, _KeyIt> &&
             std::is_convertible_v<_M2, _MappedIt>
  _LIBCPP_HIDE_FROM_ABI __flat_map_iterator(const __flat_map_iterator<_K2, _M2>& __rhs)
    : __kt_(__rhs.__key()), __vt_(__rhs.__value()) {}

  template <class _K2, class _M2>
    requires std::is_convertible_v<_K2, _KeyIt> &&
             std::is_convertible_v<_M2, _MappedIt>
  _LIBCPP_HIDE_FROM_ABI __flat_map_iterator(__flat_map_iterator<_K2, _M2>&& __rhs)
    : __kt_(std::move(__rhs.__key())), __vt_(std::move(__rhs.__value())) {}

  _LIBCPP_HIDE_FROM_ABI
  explicit __flat_map_iterator(_KeyIt __kt, _MappedIt __vt)
    : __kt_(__kt), __vt_(__vt) {}

  _LIBCPP_HIDE_FROM_ABI reference operator*() const { return {*__kt_, *__vt_}; }
  _LIBCPP_HIDE_FROM_ABI __arrow_proxy<reference> operator->() const { return __arrow_proxy<reference>(**this); }
  _LIBCPP_HIDE_FROM_ABI reference operator[](difference_type __n) const { return *(*this + __n); }
  friend _LIBCPP_HIDE_FROM_ABI __rvalue_reference iter_move(__flat_map_iterator __it) {
    return __rvalue_reference(ranges::iter_move(__it.__kt_), ranges::iter_move(__it.__vt_));
  }
#if 0 // XXX
  friend _LIBCPP_HIDE_FROM_ABI void iter_swap(__flat_map_iterator __it, __flat_map_iterator __jt) {
    ranges::swap(*__it, *__jt);
  }
#endif

  _LIBCPP_HIDE_FROM_ABI __flat_map_iterator& operator++() { ++__kt_; ++__vt_; return *this; }
  _LIBCPP_HIDE_FROM_ABI __flat_map_iterator& operator--() { --__kt_; --__vt_; return *this; }
  _LIBCPP_HIDE_FROM_ABI __flat_map_iterator operator++(int) { auto __copy = *this; ++*this; return __copy; }
  _LIBCPP_HIDE_FROM_ABI __flat_map_iterator operator--(int) { auto __copy = *this; --*this; return __copy; }
  _LIBCPP_HIDE_FROM_ABI __flat_map_iterator& operator+=(difference_type __n) { __kt_ += __n; __vt_ += __n; return *this; }
  _LIBCPP_HIDE_FROM_ABI __flat_map_iterator& operator-=(difference_type __n) { __kt_ -= __n; __vt_ -= __n; return *this; }

  friend _LIBCPP_HIDE_FROM_ABI __flat_map_iterator operator+(__flat_map_iterator __it, ptrdiff_t __n) {
    __it += __n;
    return __it;
  }
  friend _LIBCPP_HIDE_FROM_ABI __flat_map_iterator operator-(__flat_map_iterator __it, ptrdiff_t __n) {
    __it -= __n;
    return __it;
  }
  friend _LIBCPP_HIDE_FROM_ABI __flat_map_iterator operator+(ptrdiff_t __n, __flat_map_iterator __it) {
    __it += __n;
    return __it;
  }
  friend _LIBCPP_HIDE_FROM_ABI ptrdiff_t operator-(__flat_map_iterator __a, __flat_map_iterator __b) {
    return __a.__kt_ - __b.__kt_;
  }

  friend _LIBCPP_HIDE_FROM_ABI bool
  operator==(const __flat_map_iterator& __a, const __flat_map_iterator& __b) {
    return __a.__kt_ == __b.__kt_;
  }

  friend _LIBCPP_HIDE_FROM_ABI strong_ordering
  operator<=>(const __flat_map_iterator& __a, const __flat_map_iterator& __b) {
    // Synthesize a strong ordering, even if _KeyIt is pre-C++20.
    if constexpr (three_way_comparable<_KeyIt, _KeyIt>) {
      return (__a.__kt_ <=> __b.__kt_);
    } else {
      return ((__a.__kt_ < __b.__kt_) ? -1 : (__a.__kt_ > __b.__kt_)) <=> 0;
    }
  }

  _LIBCPP_HIDE_FROM_ABI _KeyIt& __key() { return __kt_; }
  _LIBCPP_HIDE_FROM_ABI const _KeyIt& __key() const { return __kt_; }
  _LIBCPP_HIDE_FROM_ABI _MappedIt& __value() { return __vt_; }
  _LIBCPP_HIDE_FROM_ABI const _MappedIt& __value() const { return __vt_; }

private:
  friend struct __product_iterator_traits<__flat_map_iterator>;

  _KeyIt __kt_ = _KeyIt();
  _MappedIt __vt_ = _MappedIt();
};

template <class _KeyIt, class _MappedIt>
struct __product_iterator_traits<__flat_map_iterator<_KeyIt, _MappedIt>> {
  using _Iterator = __flat_map_iterator<_KeyIt, _MappedIt>;
  static constexpr size_t __size = 2;

  template <size_t _Nth, class _Iter>
    requires (_Nth == 0)
  _LIBCPP_HIDE_FROM_ABI static constexpr auto&& __get_iterator_element(_Iter&& __it) {
    return __it.__kt_;
  }

  template <size_t _Nth, class _Iter>
    requires (_Nth == 1)
  _LIBCPP_HIDE_FROM_ABI static constexpr auto&& __get_iterator_element(_Iter&& __it) {
    return __it.__vt_;
  }

  template <class... _Iters>
  _LIBCPP_HIDE_FROM_ABI static constexpr _Iterator __make_product_iterator(_Iters&&... __its) {
    return _Iterator(std::forward<_Iters>(__its)...);
  }
};

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP_STD_VER >= 23

_LIBCPP_POP_MACROS

#endif // _LIBCPP___FLAT_MAP_FLAT_MAP_ITERATOR_H
