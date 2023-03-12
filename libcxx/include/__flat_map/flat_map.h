// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___FLAT_SET_FLAT_MAP_H
#define _LIBCPP___FLAT_SET_FLAT_MAP_H

/*

namespace std {
  template<class Key, class T, class Compare = less<Key>,
           class KeyContainer = vector<Key>, class MappedContainer = vector<T>>
  class flat_map {
  public:
    // types
    using key_type               = Key;
    using mapped_type            = T;
    using value_type             = pair<key_type, mapped_type>;
    using key_compare            = Compare;
    using reference              = pair<const key_type&, mapped_type&>;
    using const_reference        = pair<const key_type&, const mapped_type&>;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;
    using iterator               = implementation-defined; // see [container.requirements]
    using const_iterator         = implementation-defined; // see [container.requirements]
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using key_container_type     = KeyContainer;
    using mapped_container_type  = MappedContainer;

    class value_compare {
    private:
      key_compare comp;                                 // exposition only
      value_compare(key_compare c) : comp(c) { }        // exposition only

    public:
      bool operator()(const_reference x, const_reference y) const {
        return comp(x.first, y.first);
      }
    };

    struct containers {
      key_container_type keys;
      mapped_container_type values;
    };

    // [flat.map.cons], construct/copy/destroy
    flat_map() : flat_map(key_compare()) { }

    flat_map(key_container_type key_cont, mapped_container_type mapped_cont,
             const key_compare& comp = key_compare());
    template<class Allocator>
      flat_map(const key_container_type& key_cont, const mapped_container_type& mapped_cont,
               const Allocator& a);
    template<class Allocator>
      flat_map(const key_container_type& key_cont, const mapped_container_type& mapped_cont,
               const key_compare& comp, const Allocator& a);

    flat_map(sorted_unique_t, key_container_type key_cont, mapped_container_type mapped_cont,
             const key_compare& comp = key_compare());
    template<class Allocator>
      flat_map(sorted_unique_t, const key_container_type& key_cont,
               const mapped_container_type& mapped_cont, const Allocator& a);
    template<class Allocator>
      flat_map(sorted_unique_t, const key_container_type& key_cont,
               const mapped_container_type& mapped_cont,
               const key_compare& comp, const Allocator& a);

    explicit flat_map(const key_compare& comp)
      : c(), compare(comp) { }
    template<class Allocator>
      flat_map(const key_compare& comp, const Allocator& a);
    template<class Allocator>
      explicit flat_map(const Allocator& a);

    template<class InputIterator>
      flat_map(InputIterator first, InputIterator last, const key_compare& comp = key_compare())
        : c(), compare(comp) { insert(first, last); }
    template<class InputIterator, class Allocator>
      flat_map(InputIterator first, InputIterator last,
               const key_compare& comp, const Allocator& a);
    template<class InputIterator, class Allocator>
      flat_map(InputIterator first, InputIterator last, const Allocator& a);

    template<container-compatible-range<value_type> R>
      flat_map(from_range_t fr, R&& rg)
        : flat_map(fr, std::forward<R>(rg), key_compare()) { }
    template<container-compatible-range<value_type> R, class Allocator>
      flat_map(from_range_t, R&& rg, const Allocator& a);
    template<container-compatible-range<value_type> R>
      flat_map(from_range_t, R&& rg, const key_compare& comp)
        : flat_map(comp) { insert_range(std::forward<R>(rg)); }
    template<container-compatible-range<value_type> R, class Allocator>
      flat_map(from_range_t, R&& rg, const key_compare& comp, const Allocator& a);

    template<class InputIterator>
      flat_map(sorted_unique_t s, InputIterator first, InputIterator last,
               const key_compare& comp = key_compare())
        : c(), compare(comp) { insert(s, first, last); }
    template<class InputIterator, class Allocator>
      flat_map(sorted_unique_t, InputIterator first, InputIterator last,
               const key_compare& comp, const Allocator& a);
    template<class InputIterator, class Allocator>
      flat_map(sorted_unique_t, InputIterator first, InputIterator last, const Allocator& a);

    flat_map(initializer_list<value_type> il, const key_compare& comp = key_compare())
        : flat_map(il.begin(), il.end(), comp) { }
    template<class Allocator>
      flat_map(initializer_list<value_type> il, const key_compare& comp, const Allocator& a);
    template<class Allocator>
      flat_map(initializer_list<value_type> il, const Allocator& a);

    flat_map(sorted_unique_t s, initializer_list<value_type> il,
             const key_compare& comp = key_compare())
        : flat_map(s, il.begin(), il.end(), comp) { }
    template<class Allocator>
      flat_map(sorted_unique_t, initializer_list<value_type> il,
               const key_compare& comp, const Allocator& a);
    template<class Allocator>
      flat_map(sorted_unique_t, initializer_list<value_type> il, const Allocator& a);

    flat_map& operator=(initializer_list<value_type> il);

    // iterators
    iterator               begin() noexcept;
    const_iterator         begin() const noexcept;
    iterator               end() noexcept;
    const_iterator         end() const noexcept;

    reverse_iterator       rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    reverse_iterator       rend() noexcept;
    const_reverse_iterator rend() const noexcept;

    const_iterator         cbegin() const noexcept;
    const_iterator         cend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // [flat.map.capacity], capacity
    [[nodiscard]] bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;

    // [flat.map.access], element access
    mapped_type& operator[](const key_type& x);
    mapped_type& operator[](key_type&& x);
    template<class K> mapped_type& operator[](K&& x);
    mapped_type& at(const key_type& x);
    const mapped_type& at(const key_type& x) const;
    template<class K> mapped_type& at(const K& x);
    template<class K> const mapped_type& at(const K& x) const;

    // [flat.map.modifiers], modifiers
    template<class... Args> pair<iterator, bool> emplace(Args&&... args);
    template<class... Args>
      iterator emplace_hint(const_iterator position, Args&&... args);

    pair<iterator, bool> insert(const value_type& x)
      { return emplace(x); }
    pair<iterator, bool> insert(value_type&& x)
      { return emplace(std::move(x)); }
    iterator insert(const_iterator position, const value_type& x)
      { return emplace_hint(position, x); }
    iterator insert(const_iterator position, value_type&& x)
      { return emplace_hint(position, std::move(x)); }

    template<class P> pair<iterator, bool> insert(P&& x);
    template<class P>
      iterator insert(const_iterator position, P&&);
    template<class InputIterator>
      void insert(InputIterator first, InputIterator last);
    template<class InputIterator>
      void insert(sorted_unique_t, InputIterator first, InputIterator last);
    template<container-compatible-range<value_type> R>
      void insert_range(R&& rg);

    void insert(initializer_list<value_type> il)
      { insert(il.begin(), il.end()); }
    void insert(sorted_unique_t s, initializer_list<value_type> il)
      { insert(s, il.begin(), il.end()); }

    containers extract() &&;
    void replace(key_container_type&& key_cont, mapped_container_type&& mapped_cont);

    template<class... Args>
      pair<iterator, bool> try_emplace(const key_type& k, Args&&... args);
    template<class... Args>
      pair<iterator, bool> try_emplace(key_type&& k, Args&&... args);
    template<class K, class... Args>
      pair<iterator, bool> try_emplace(K&& k, Args&&... args);
    template<class... Args>
      iterator try_emplace(const_iterator hint, const key_type& k, Args&&... args);
    template<class... Args>
      iterator try_emplace(const_iterator hint, key_type&& k, Args&&... args);
    template<class K, class... Args>
      iterator try_emplace(const_iterator hint, K&& k, Args&&... args);
    template<class M>
      pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj);
    template<class M>
      pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj);
    template<class K, class M>
      pair<iterator, bool> insert_or_assign(K&& k, M&& obj);
    template<class M>
      iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj);
    template<class M>
      iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj);
    template<class K, class M>
      iterator insert_or_assign(const_iterator hint, K&& k, M&& obj);

    iterator erase(iterator position);
    iterator erase(const_iterator position);
    size_type erase(const key_type& x);
    template<class K> size_type erase(K&& x);
    iterator erase(const_iterator first, const_iterator last);

    void swap(flat_map& y) noexcept;
    void clear() noexcept;

    // observers
    key_compare key_comp() const;
    value_compare value_comp() const;

    const key_container_type& keys() const noexcept      { return c.keys; }
    const mapped_container_type& values() const noexcept { return c.values; }

    // map operations
    iterator find(const key_type& x);
    const_iterator find(const key_type& x) const;
    template<class K> iterator find(const K& x);
    template<class K> const_iterator find(const K& x) const;

    size_type count(const key_type& x) const;
    template<class K> size_type count(const K& x) const;

    bool contains(const key_type& x) const;
    template<class K> bool contains(const K& x) const;

    iterator lower_bound(const key_type& x);
    const_iterator lower_bound(const key_type& x) const;
    template<class K> iterator lower_bound(const K& x);
    template<class K> const_iterator lower_bound(const K& x) const;

    iterator upper_bound(const key_type& x);
    const_iterator upper_bound(const key_type& x) const;
    template<class K> iterator upper_bound(const K& x);
    template<class K> const_iterator upper_bound(const K& x) const;

    pair<iterator, iterator> equal_range(const key_type& x);
    pair<const_iterator, const_iterator> equal_range(const key_type& x) const;
    template<class K> pair<iterator, iterator> equal_range(const K& x);
    template<class K> pair<const_iterator, const_iterator> equal_range(const K& x) const;

    friend bool operator==(const flat_map& x, const flat_map& y);

    friend synth-three-way-result<value_type>
      operator<=>(const flat_map& x, const flat_map& y);

    friend void swap(flat_map& x, flat_map& y) noexcept
      { x.swap(y); }

  private:
    containers c;               // exposition only
    key_compare compare;        // exposition only

    struct key_equiv {  // exposition only
      key_equiv(key_compare c) : comp(c) { }
      bool operator()(const_reference x, const_reference y) const {
        return !comp(x.first, y.first) && !comp(y.first, x.first);
      }
      key_compare comp;
    };
  };

  template<class KeyContainer, class MappedContainer,
           class Compare = less<typename KeyContainer::value_type>>
    flat_map(KeyContainer, MappedContainer, Compare = Compare())
      -> flat_map<typename KeyContainer::value_type, typename MappedContainer::value_type,
                  Compare, KeyContainer, MappedContainer>;

  template<class KeyContainer, class MappedContainer, class Allocator>
    flat_map(KeyContainer, MappedContainer, Allocator)
      -> flat_map<typename KeyContainer::value_type, typename MappedContainer::value_type,
                  less<typename KeyContainer::value_type>, KeyContainer, MappedContainer>;
  template<class KeyContainer, class MappedContainer, class Compare, class Allocator>
    flat_map(KeyContainer, MappedContainer, Compare, Allocator)
      -> flat_map<typename KeyContainer::value_type, typename MappedContainer::value_type,
                  Compare, KeyContainer, MappedContainer>;

  template<class KeyContainer, class MappedContainer,
           class Compare = less<typename KeyContainer::value_type>>
    flat_map(sorted_unique_t, KeyContainer, MappedContainer, Compare = Compare())
      -> flat_map<typename KeyContainer::value_type, typename MappedContainer::value_type,
                  Compare, KeyContainer, MappedContainer>;

  template<class KeyContainer, class MappedContainer, class Allocator>
    flat_map(sorted_unique_t, KeyContainer, MappedContainer, Allocator)
      -> flat_map<typename KeyContainer::value_type, typename MappedContainer::value_type,
                  less<typename KeyContainer::value_type>, KeyContainer, MappedContainer>;
  template<class KeyContainer, class MappedContainer, class Compare, class Allocator>
    flat_map(sorted_unique_t, KeyContainer, MappedContainer, Compare, Allocator)
      -> flat_map<typename KeyContainer::value_type, typename MappedContainer::value_type,
                  Compare, KeyContainer, MappedContainer>;

  template<class InputIterator, class Compare = less<iter-key-type<InputIterator>>>
    flat_map(InputIterator, InputIterator, Compare = Compare())
      -> flat_map<iter-key-type<InputIterator>, iter-mapped-type<InputIterator>, Compare>;

  template<class InputIterator, class Compare = less<iter-key-type<InputIterator>>>
    flat_map(sorted_unique_t, InputIterator, InputIterator, Compare = Compare())
      -> flat_map<iter-key-type<InputIterator>, iter-mapped-type<InputIterator>, Compare>;

  template<ranges::input_range R, class Compare = less<range-key-type<R>>,
           class Allocator = allocator<byte>>
    flat_map(from_range_t, R&&, Compare = Compare(), Allocator = Allocator())
      -> flat_map<range-key-type<R>, range-mapped-type<R>, Compare,
                  vector<range-key-type<R>, alloc-rebind<Allocator, range-key-type<R>>>,
                  vector<range-mapped-type<R>, alloc-rebind<Allocator, range-mapped-type<R>>>>;

  template<ranges::input_range R, class Allocator>
    flat_map(from_range_t, R&&, Allocator)
      -> flat_map<range-key-type<R>, range-mapped-type<R>, less<range-key-type<R>>,
                  vector<range-key-type<R>, alloc-rebind<Allocator, range-key-type<R>>>,
                  vector<range-mapped-type<R>, alloc-rebind<Allocator, range-mapped-type<R>>>>;

  template<class Key, class T, class Compare = less<Key>>
    flat_map(initializer_list<pair<Key, T>>, Compare = Compare())
      -> flat_map<Key, T, Compare>;

  template<class Key, class T, class Compare = less<Key>>
    flat_map(sorted_unique_t, initializer_list<pair<Key, T>>, Compare = Compare())
        -> flat_map<Key, T, Compare>;

  template<class Key, class T, class Compare, class KeyContainer, class MappedContainer,
            class Allocator>
    struct uses_allocator<flat_map<Key, T, Compare, KeyContainer, MappedContainer>, Allocator>
      : bool_constant<uses_allocator_v<KeyContainer, Allocator> &&
                      uses_allocator_v<MappedContainer, Allocator>> { };
}

*/

#include <__algorithm/adjacent_find.h>
#include <__algorithm/lexicographical_compare_three_way.h>
#include <__algorithm/partition_point.h>
#include <__algorithm/ranges_inplace_merge.h>
#include <__algorithm/ranges_stable_sort.h>
#include <__algorithm/remove_if.h>
#include <__assert> // all public C++ headers provide the assertion handler
#include <__compare/synth_three_way.h>
#include <__config>
#include <__functional/is_transparent.h>
#include <__functional/operations.h>
#include <__functional/reference_wrapper.h>
#include <__iterator/iter_move.h>
#include <__iterator/iterator_traits.h>
#include <__iterator/ranges_iterator_traits.h>
#include <__iterator/readable_traits.h>
#include <__iterator/reverse_iterator.h>
#include <__memory/allocator.h>
#include <__memory/allocator_traits.h>
#include <__memory/uses_allocator.h>
#include <__memory/uses_allocator_construction.h>
#include <__ranges/access.h>
#include <__ranges/concepts.h>
#include <__ranges/container_compatible_range.h>
#include <__ranges/from_range.h>
#include <__type_traits/is_allocator.h>
#include <__type_traits/is_assignable.h>
#include <__type_traits/is_constructible.h>
#include <__type_traits/is_convertible.h>
#include <__type_traits/is_nothrow_assignable.h>
#include <__type_traits/is_nothrow_constructible.h>
#include <__type_traits/is_same.h>
#include <__type_traits/is_trivially_relocatable.h>
#include <__type_traits/type_identity.h>
#include <__utility/arrow_proxy.h>
#include <__utility/exception_guard.h>
#include <__utility/forward.h>
#include <__utility/move.h>
#include <__utility/pair.h>
#include <__utility/sorted_unique.h>
#include <vector>
#include <version>

// standard-mandated includes

// [flat.set.syn]
#include <compare>
#include <initializer_list>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_PUSH_MACROS
#include <__undef_macros>

#if _LIBCPP_STD_VER >= 23

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _FwdIt, class _Compare>
_LIBCPP_HIDE_FROM_ABI _FwdIt
__flatmap_unique(_FwdIt __first, _FwdIt __last, _Compare __value_comp) {
  // This algorithm differs from std::unique in two ways:
  // - __flatmap_unique gets to assume that the range is already sorted
  // - std::unique has a Precondition that __comp is an equivalence relation,
  //   whereas for __flatmap_unique it is a less-than comparator
  //
  if (__first == __last)
    return __first;
  _FwdIt __dest = __first;
  for (++__first; __first != __last; ++__first) {
    if (__value_comp(*__dest, *__first)) {
      ++__dest;
      if (__dest != __first) {
        *__dest = ranges::iter_move(__first);
      }
    }
  }
  ++__dest;
  return __dest;
}

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
  _KeyIt __kt_ = _KeyIt();
  _MappedIt __vt_ = _MappedIt();
};

template <class _Compare>
class _LIBCPP_TEMPLATE_VIS __flatmap_value_comp_ref {
public:
  _LIBCPP_HIDE_FROM_ABI
  explicit __flatmap_value_comp_ref(const _Compare& __c) : __comp_(__c) { }

  template<class _Xp, class _Yp>
  _LIBCPP_HIDE_FROM_ABI bool operator()(const _Xp& __x, const _Yp& __y) const {
    return __comp_(__x.first, __y.first);
  }
private:
  const _Compare& __comp_;
};

template <class _Key, class _Tp, class _Compare = less<_Key>,
          class _KeyContainer = vector<_Key>, class _MappedContainer = vector<_Tp>>
class _LIBCPP_TRIVIALLY_RELOCATABLE_IF(
  __libcpp_is_trivially_relocatable<_KeyContainer>::value &&
  __libcpp_is_trivially_relocatable<_MappedContainer>::value &&
  __libcpp_is_trivially_relocatable<_Compare>::value
) flat_map {
  static_assert(is_same_v<_Key, typename _KeyContainer::value_type>);
  static_assert(is_same_v<_Tp, typename _MappedContainer::value_type>);
  static_assert(__has_random_access_iterator_category<typename _KeyContainer::iterator>::value);
  static_assert(__has_random_access_iterator_category<typename _MappedContainer::iterator>::value);

public:
  using iterator = __flat_map_iterator<typename _KeyContainer::const_iterator, typename _MappedContainer::iterator>;
  using const_iterator = __flat_map_iterator<typename _KeyContainer::const_iterator, typename _MappedContainer::const_iterator>;
  using key_type = _Key;
  using mapped_type = _Tp;
  using value_type = pair<_Key, _Tp>;
  using key_compare = _Compare;
  using reference = typename iterator::reference;
  using const_reference = typename const_iterator::reference;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using key_container_type = _KeyContainer;
  using mapped_container_type = _MappedContainer;
  using __mutable_iterator = __flat_map_iterator<typename _KeyContainer::iterator, typename _MappedContainer::iterator>;
  using __mapped_reference = iter_reference_t<typename _MappedContainer::iterator>;
  using __mapped_const_reference = iter_reference_t<typename _MappedContainer::const_iterator>;

  class _LIBCPP_TEMPLATE_VIS value_compare {
  public:
    _LIBCPP_HIDE_FROM_ABI
    bool operator()(const_reference __x, const_reference __y) const {
      return __comp_(__x.first, __y.first);
    }
  private:
    _LIBCPP_HIDE_FROM_ABI
    explicit value_compare(key_compare __c) : __comp_(__c) { }

    friend class flat_map;

    key_compare __comp_;
  };

  struct containers {
    key_container_type keys;
    mapped_container_type values;
  };

  // [flat.map.cons]

  _LIBCPP_HIDE_FROM_ABI
  flat_map()
    noexcept(is_nothrow_default_constructible_v<key_container_type> &&
             is_nothrow_default_constructible_v<mapped_container_type> &&
             is_nothrow_default_constructible_v<key_compare>) // strengthened
    = default;


  _LIBCPP_HIDE_FROM_ABI
  flat_map(key_container_type __keys,
           mapped_container_type __values,
           const key_compare& __comp = key_compare())
    : __compare_(__comp), __keys_(std::move(__keys)), __values_(std::move(__values)) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__keys_.size() == __values_.size(),
        "key_cont and mapped_cont must be equal in size");
    __construct_invariant();
  }

  template <class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(const key_container_type& __keys, const mapped_container_type& __values,
           const _Alloc& __a)
    : flat_map(__keys, __values, key_compare(), __a) { }

  template <class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(const key_container_type& __keys, mapped_container_type&& __values,
           const _Alloc& __a)
    : flat_map(__keys, std::move(__values), key_compare(), __a) { }

  template <class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(key_container_type&& __keys, const mapped_container_type& __values,
           const _Alloc& __a)
    : flat_map(std::move(__keys), __values, key_compare(), __a) { }

  template <class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(key_container_type&& __keys, mapped_container_type&& __values,
           const _Alloc& __a)
    : flat_map(std::move(__keys), std::move(__values), key_compare(), __a) { }

  template<class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(const key_container_type& __keys, const mapped_container_type& __values,
           const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp),
      __keys_(std::__make_obj_using_allocator<key_container_type>(__a, __keys)),
      __values_(std::__make_obj_using_allocator<mapped_container_type>(__a, __values)) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__keys_.size() == __values_.size(),
        "key_cont and mapped_cont must be equal in size");
    __construct_invariant();
  }

  template<class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(const key_container_type& __keys, mapped_container_type&& __values,
           const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp),
      __keys_(std::__make_obj_using_allocator<key_container_type>(__a, __keys)),
      __values_(std::__make_obj_using_allocator<mapped_container_type>(__a, std::move(__values))) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__keys_.size() == __values_.size(),
        "key_cont and mapped_cont must be equal in size");
    __construct_invariant();
  }

  template<class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(key_container_type&& __keys, const mapped_container_type& __values,
           const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp),
      __keys_(std::__make_obj_using_allocator<key_container_type>(__a, std::move(__keys))),
      __values_(std::__make_obj_using_allocator<mapped_container_type>(__a, __values)) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__keys_.size() == __values_.size(),
        "key_cont and mapped_cont must be equal in size");
    __construct_invariant();
  }

  template<class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(key_container_type&& __keys, mapped_container_type&& __values,
           const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp),
      __keys_(std::__make_obj_using_allocator<key_container_type>(__a, std::move(__keys))),
      __values_(std::__make_obj_using_allocator<mapped_container_type>(__a, std::move(__values))) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__keys_.size() == __values_.size(),
        "key_cont and mapped_cont must be equal in size");
    __construct_invariant();
  }

  _LIBCPP_HIDE_FROM_ABI
  flat_map(sorted_unique_t, key_container_type __keys,
           mapped_container_type __values,
           const key_compare& __comp = key_compare())
    : __compare_(__comp), __keys_(std::move(__keys)), __values_(std::move(__values)) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__keys_.size() == __values_.size(),
        "key_cont and mapped_cont must be equal in size");
    _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted_uniqued(begin(), end()),
        "sorted_unique expects input that is sorted and uniqued");
  }

  template <class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(sorted_unique_t __s, const key_container_type& __keys, const mapped_container_type& __values,
           const _Alloc& __a)
    : flat_map(__s, __keys, __values, key_compare(), __a) { }

  template <class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(sorted_unique_t __s, const key_container_type& __keys, mapped_container_type&& __values,
           const _Alloc& __a)
    : flat_map(__s, __keys, std::move(__values), key_compare(), __a) { }

  template <class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(sorted_unique_t __s, key_container_type&& __keys, const mapped_container_type& __values,
           const _Alloc& __a)
    : flat_map(__s, std::move(__keys), __values, key_compare(), __a) { }

  template <class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(sorted_unique_t __s, key_container_type&& __keys, mapped_container_type&& __values,
           const _Alloc& __a)
    : flat_map(__s, std::move(__keys), std::move(__values), key_compare(), __a) { }

  template<class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(sorted_unique_t, const key_container_type& __keys, const mapped_container_type& __values,
           const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp),
      __keys_(std::__make_obj_using_allocator<key_container_type>(__a, __keys)),
      __values_(std::__make_obj_using_allocator<mapped_container_type>(__a, __values)) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__keys_.size() == __values_.size(),
        "key_cont and mapped_cont must be equal in size");
    _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted_uniqued(begin(), end()),
        "sorted_unique expects input that is sorted and uniqued");
  }

  template<class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(sorted_unique_t, const key_container_type& __keys, mapped_container_type&& __values,
           const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp),
      __keys_(std::__make_obj_using_allocator<key_container_type>(__a, __keys)),
      __values_(std::__make_obj_using_allocator<mapped_container_type>(__a, std::move(__values))) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__keys_.size() == __values_.size(),
        "key_cont and mapped_cont must be equal in size");
    _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted_uniqued(begin(), end()),
        "sorted_unique expects input that is sorted and uniqued");
  }

  template<class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(sorted_unique_t, key_container_type&& __keys, const mapped_container_type& __values,
           const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp),
      __keys_(std::__make_obj_using_allocator<key_container_type>(__a, std::move(__keys))),
      __values_(std::__make_obj_using_allocator<mapped_container_type>(__a, __values)) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__keys_.size() == __values_.size(),
        "key_cont and mapped_cont must be equal in size");
    _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted_uniqued(begin(), end()),
        "sorted_unique expects input that is sorted and uniqued");
  }

  template<class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(sorted_unique_t, key_container_type&& __keys, mapped_container_type&& __values,
           const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp),
      __keys_(std::__make_obj_using_allocator<key_container_type>(__a, std::move(__keys))),
      __values_(std::__make_obj_using_allocator<mapped_container_type>(__a, std::move(__values))) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__keys_.size() == __values_.size(),
        "key_cont and mapped_cont must be equal in size");
    _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted_uniqued(begin(), end()),
        "sorted_unique expects input that is sorted and uniqued");
  }

  _LIBCPP_HIDE_FROM_ABI
  flat_map(const flat_map& __fm) = default;

  _LIBCPP_HIDE_FROM_ABI
  flat_map(flat_map&& __fm)
    noexcept(is_nothrow_constructible_v<key_compare, key_compare&> &&
             is_nothrow_move_constructible_v<key_container_type> &&
             is_nothrow_move_constructible_v<mapped_container_type>) // strengthened
#ifndef _LIBCPP_HAS_NO_EXCEPTIONS
    try
#endif
    : __compare_(__fm.__compare_), __keys_(std::move(__fm.__keys_)), __values_(std::move(__fm.__values_)) {
    __fm.clear();
  }
#ifndef _LIBCPP_HAS_NO_EXCEPTIONS
  catch (...) {
    __fm.__restore_invariant();
    throw;
  }
#endif

  _LIBCPP_HIDE_FROM_ABI
  explicit flat_map(const key_compare& __comp)
    : __compare_(__comp) {}

  template <class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp), __keys_(std::__make_obj_using_allocator<key_container_type>(__a)),
      __values_(std::__make_obj_using_allocator<mapped_container_type>(__a)) {}

  template <class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  explicit flat_map(const _Alloc& __a)
    : __keys_(std::__make_obj_using_allocator<key_container_type>(__a)),
      __values_(std::__make_obj_using_allocator<mapped_container_type>(__a)) {}

  template <class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(const flat_map& __fm, const _Alloc& __a)
    : __compare_(__fm.__compare_),
      __keys_(std::__make_obj_using_allocator<key_container_type>(__a, __fm.__keys_)),
      __values_(std::__make_obj_using_allocator<mapped_container_type>(__a, __fm.__values_)) {}

  template <class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(flat_map&& __fm, const _Alloc& __a)
#ifndef _LIBCPP_HAS_NO_EXCEPTIONS
    try
#endif
    : __compare_(__fm.__compare_),
      __keys_(std::__make_obj_using_allocator<key_container_type>(__a, std::move(__fm.__keys_))),
      __values_(std::__make_obj_using_allocator<mapped_container_type>(__a, std::move(__fm.__values_))) {
    __fm.clear();
  }
#ifndef _LIBCPP_HAS_NO_EXCEPTIONS
  catch (...) {
    __fm.__restore_invariant();
    throw;
  }
#endif

  template <class _InputIterator>
    requires __has_input_iterator_category<_InputIterator>::value
  _LIBCPP_HIDE_FROM_ABI
  flat_map(_InputIterator __first, _InputIterator __last, const key_compare& __comp = key_compare())
    : __compare_(__comp) {
    insert(__first, __last);
  }

  template <class _InputIterator, class _Alloc>
    requires __has_input_iterator_category<_InputIterator>::value &&
             uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(_InputIterator __first, _InputIterator __last, const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp),
      __keys_(std::__make_obj_using_allocator<key_container_type>(__a)),
      __values_(std::__make_obj_using_allocator<mapped_container_type>(__a)) {
    insert(__first, __last);
  }

  template <class _InputIterator, class _Alloc>
    requires __has_input_iterator_category<_InputIterator>::value &&
             uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(_InputIterator __first, _InputIterator __last, const _Alloc& __a)
    : __keys_(std::__make_obj_using_allocator<key_container_type>(__a)),
      __values_(std::__make_obj_using_allocator<mapped_container_type>(__a)) {
    insert(__first, __last);
  }

  template <_ContainerCompatibleRange<value_type> _Rp>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(from_range_t, _Rp&& __r, const key_compare& __comp = key_compare())
    : flat_map(__comp) {
    insert_range(std::forward<_Rp>(__r));
  }

  template <_ContainerCompatibleRange<value_type> _Rp, class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(from_range_t __fr, _Rp&& __r, const _Alloc& __a)
    : flat_map(__fr, std::forward<_Rp>(__r), key_compare(), __a) {}

  template <_ContainerCompatibleRange<value_type> _Rp, class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(from_range_t, _Rp&& __r, const key_compare& __comp, const _Alloc& __a)
    : flat_map(__comp, __a) {
    insert_range(std::forward<_Rp>(__r));
  }

  template <class _InputIterator>
    requires __has_input_iterator_category<_InputIterator>::value
  _LIBCPP_HIDE_FROM_ABI
  flat_map(sorted_unique_t __s, _InputIterator __first, _InputIterator __last, const key_compare& __comp = key_compare())
    : __compare_(__comp) {
    insert(__s, __first, __last);
  }

  template <class _InputIterator, class _Alloc>
    requires __has_input_iterator_category<_InputIterator>::value &&
             uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(sorted_unique_t __s, _InputIterator __first, _InputIterator __last, const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp),
      __keys_(std::__make_obj_using_allocator<key_container_type>(__a)),
      __values_(std::__make_obj_using_allocator<mapped_container_type>(__a)) {
    insert(__s, __first, __last);
  }

  template <class _InputIterator, class _Alloc>
    requires __has_input_iterator_category<_InputIterator>::value &&
             uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(sorted_unique_t __s, _InputIterator __first, _InputIterator __last, const _Alloc& __a)
    : __keys_(std::__make_obj_using_allocator<key_container_type>(__a)),
      __values_(std::__make_obj_using_allocator<mapped_container_type>(__a)) {
    insert(__s, __first, __last);
  }

  _LIBCPP_HIDE_FROM_ABI
  flat_map(initializer_list<value_type> __il, const key_compare& __comp = key_compare())
    : flat_map(__il.begin(), __il.end(), __comp) {}

  template <class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(initializer_list<value_type> __il, const type_identity_t<key_compare>& __comp, const _Alloc& __a)
    : flat_map(__il.begin(), __il.end(), __comp, __a) {}

  template <class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(initializer_list<value_type> __il, const _Alloc& __a)
    : flat_map(__il.begin(), __il.end(), __a) {}

  _LIBCPP_HIDE_FROM_ABI
  flat_map(sorted_unique_t __s, initializer_list<value_type> __il,
         const key_compare& __comp = key_compare())
    : flat_map(__s, __il.begin(), __il.end(), __comp) {}

  template <class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(sorted_unique_t __s, initializer_list<value_type> __il,
           const type_identity_t<key_compare>& __comp, const _Alloc& __a)
    : flat_map(__s, __il.begin(), __il.end(), __comp, __a) {}

  template <class _Alloc>
    requires uses_allocator_v<key_container_type, _Alloc> &&
             uses_allocator_v<mapped_container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_map(sorted_unique_t __s, initializer_list<value_type> __il, const _Alloc& __a)
    : flat_map(__s, __il.begin(), __il.end(), __a) { }

  _LIBCPP_HIDE_FROM_ABI
  flat_map& operator=(initializer_list<value_type> __il) {
    clear();
    insert(__il.begin(), __il.end());
    return *this;
  }

  _LIBCPP_HIDE_FROM_ABI
  flat_map& operator=(const flat_map& __fm) {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    __compare_ = __fm.__compare_;
    __keys_ = __fm.__keys_;
    __values_ = __fm.__values_;
    __guard.__complete();
    return *this;
  }

  _LIBCPP_HIDE_FROM_ABI
  flat_map& operator=(flat_map&& __fm)
    noexcept(is_nothrow_assignable_v<_Compare&, _Compare&> &&
             is_nothrow_assignable_v<_KeyContainer&, _KeyContainer&&> &&
             is_nothrow_assignable_v<_MappedContainer&, _MappedContainer&&>) // strengthened
  {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    __compare_ = __fm.__compare_;
    auto __guard2 = std::__make_exception_guard([&] { __fm.__restore_invariant(); });
    __keys_ = std::move(__fm.__keys_);
    __values_ = std::move(__fm.__values_);
    __guard.__complete();
    // deliberately do not call __guard2.__complete(), so that the clear always happens
    return *this;
  }

  // iterators

  _LIBCPP_HIDE_FROM_ABI iterator begin() noexcept { return iterator(__keys_.begin(), __values_.begin()); }
  _LIBCPP_HIDE_FROM_ABI const_iterator begin() const noexcept { return const_iterator(__keys_.begin(), __values_.begin()); }
  _LIBCPP_HIDE_FROM_ABI iterator end() noexcept { return iterator(__keys_.end(), __values_.end()); }
  _LIBCPP_HIDE_FROM_ABI const_iterator end() const noexcept { return const_iterator(__keys_.end(), __values_.end()); }

  _LIBCPP_HIDE_FROM_ABI reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  _LIBCPP_HIDE_FROM_ABI const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
  _LIBCPP_HIDE_FROM_ABI reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
  _LIBCPP_HIDE_FROM_ABI const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

  _LIBCPP_HIDE_FROM_ABI const_iterator cbegin() const noexcept { return begin(); }
  _LIBCPP_HIDE_FROM_ABI const_iterator cend() const noexcept { return end(); }
  _LIBCPP_HIDE_FROM_ABI const_reverse_iterator crbegin() const noexcept { return rbegin(); }
  _LIBCPP_HIDE_FROM_ABI const_reverse_iterator crend() const noexcept { return rend(); }

  // [flat.map.capacity]

  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI bool empty() const noexcept { return __keys_.empty(); }
  _LIBCPP_HIDE_FROM_ABI size_type size() const noexcept { return __keys_.size(); }
  _LIBCPP_HIDE_FROM_ABI size_type max_size() const noexcept {
    return std::min<size_type>(__keys_.max_size(), __values_.max_size());
  }

  // [flat.map.access]

  _LIBCPP_HIDE_FROM_ABI
  __mapped_reference operator[](const key_type& __x) {
    return (*try_emplace(__x).first).second;
  }

  _LIBCPP_HIDE_FROM_ABI
  __mapped_reference operator[](key_type&& __x) {
    return (*try_emplace(std::move(__x)).first).second;
  }

  template<class _K2>
    requires __is_transparent_v<_Compare, _K2>
  _LIBCPP_HIDE_FROM_ABI
  __mapped_reference operator[](_K2&& __x) {
    return (*try_emplace(std::forward<_K2>(__x)).first).second;
  }

  _LIBCPP_HIDE_FROM_ABI
  __mapped_reference at(const key_type& __x) {
    auto __it = find(__x);
    if (__it == end()) {
      __throw_out_of_range("flat_map::at: key not found");
    }
    return (*__it).second;
  }

  _LIBCPP_HIDE_FROM_ABI
  __mapped_const_reference at(const key_type& __x) const {
    auto __it = find(__x);
    if (__it == end()) {
      __throw_out_of_range("flat_map::at: key not found");
    }
    return (*__it).second;
  }

  template<class _K2>
    requires __is_transparent_v<_Compare, _K2>
  _LIBCPP_HIDE_FROM_ABI
  __mapped_reference at(const _K2& __x) {
    auto __it = find(std::forward<_K2>(__x));
    if (__it == end()) {
      __throw_out_of_range("flat_map::at: key not found");
    }
    return (*__it).second;
  }

  template<class _K2>
    requires __is_transparent_v<_Compare, _K2>
  _LIBCPP_HIDE_FROM_ABI
  __mapped_const_reference at(const _K2& __x) const {
    auto __it = find(std::forward<_K2>(__x));
    if (__it == end()) {
      __throw_out_of_range("flat_map::at: key not found");
    }
    return (*__it).second;
  }

  // [flat.map.modifiers]

  template <class... _Args>
  _LIBCPP_HIDE_FROM_ABI
  pair<iterator, bool> emplace(_Args&&... __args) {
    // Notice that we construct this value_type with the wrong allocator at first.
    // That seems to be unavoidable, since the container might be allocator-unaware.
    value_type __tmp = value_type(std::forward<_Args>(__args)...);
    return insert(std::move(__tmp));
  }

  template <class... _Args>
  _LIBCPP_HIDE_FROM_ABI
  iterator emplace_hint(const_iterator __pos, _Args&&... __args) {
    // Notice that we construct this value_type with the wrong allocator at first.
    // That seems to be unavoidable, since the container might be allocator-unaware.
    value_type __tmp = value_type(std::forward<_Args>(__args)...);
    return insert(__pos, std::move(__tmp));
  }

  _LIBCPP_HIDE_FROM_ABI
  pair<iterator, bool> insert(const value_type& __x) {
    auto __pos = lower_bound(__x.first);
    if (__pos == end() || __value_comp_ref()(__x, *__pos)) {
      return {__insert_unchecked(__pos, __x), true};
    } else {
      return {__pos, false};
    }
  }

  _LIBCPP_HIDE_FROM_ABI
  pair<iterator, bool> insert(value_type&& __x) {
    auto __pos = lower_bound(__x.first);
    if (__pos == end() || __value_comp_ref()(__x, *__pos)) {
      return {__insert_unchecked(__pos, std::move(__x)), true};
    } else {
      return {__pos, false};
    }
  }

  template <class _Pair>
    requires is_constructible_v<value_type, _Pair>
  _LIBCPP_HIDE_FROM_ABI
  pair<iterator, bool> insert(_Pair&& __p) {
    return insert(value_type(std::forward<_Pair>(__p)));
  }

  _LIBCPP_HIDE_FROM_ABI
  iterator insert(const_iterator __pos, const value_type& __x) {
    if (__pos == end() || __value_comp_ref()(__x, *__pos)) {
      if (__pos == begin() || __value_comp_ref()(__pos[-1], __x)) {
        return __insert_unchecked(__pos, __x);
      }
    }
    return insert(__x).first;
  }

  _LIBCPP_HIDE_FROM_ABI
  iterator insert(const_iterator __pos, value_type&& __x) {
    if (__pos == end() || __value_comp_ref()(__x, *__pos)) {
      if (__pos == begin() || __value_comp_ref()(__pos[-1], __x)) {
        return __insert_unchecked(__pos, std::move(__x));
      }
    }
    return insert(std::move(__x)).first;
  }

  template <class _Pair>
    requires is_constructible_v<value_type, _Pair>
  _LIBCPP_HIDE_FROM_ABI
  iterator insert(const_iterator __pos, _Pair&& __p) {
    return insert(__pos, value_type(std::forward<_Pair>(__p)));
  }

  template <class _InputIterator>
    requires __has_input_iterator_category<_InputIterator>::value
  _LIBCPP_HIDE_FROM_ABI
  void insert(_InputIterator __first, _InputIterator __last) {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    size_type __n = __append_pairs(__first, __last);
    if (__n != 0) {
      auto __end = __mutable_end();
      auto __it = __end - __n;
      ranges::stable_sort(__it, __end, __value_comp_ref());
      auto __begin = __mutable_begin();
      ranges::inplace_merge(__begin, __it, __end, __value_comp_ref());
      __it = std::__flatmap_unique(__begin, __end, __value_comp_ref());
      __keys_.erase(__it.__key(), __end.__key());
      __values_.erase(__it.__value(), __end.__value());
    }
    __guard.__complete();
  }

  template <class _InputIterator>
    requires __has_input_iterator_category<_InputIterator>::value
  _LIBCPP_HIDE_FROM_ABI
  void insert(sorted_unique_t, _InputIterator __first, _InputIterator __last) {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    size_type __n = __append_pairs(__first, __last);
    if (__n != 0) {
      auto __end = __mutable_end();
      auto __it = __end - __n;
      _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted_uniqued(__it, __end),
          "sorted_unique expects input that is sorted and uniqued");
      auto __begin = __mutable_begin();
      ranges::inplace_merge(__begin, __it, __end, __value_comp_ref());
      __it = std::__flatmap_unique(__begin, __end, __value_comp_ref());
      __keys_.erase(__it.__key(), __end.__key());
      __values_.erase(__it.__value(), __end.__value());
    }
    __guard.__complete();
  }

  template <_ContainerCompatibleRange<value_type> _Rp>
  _LIBCPP_HIDE_FROM_ABI
  void insert_range(_Rp&& __r) {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    size_type __n = __append_pairs(ranges::begin(__r), ranges::end(__r));
    if (__n != 0) {
      auto __end = __mutable_end();
      auto __it = __end - __n;
      ranges::stable_sort(__it, __end, __value_comp_ref());
      auto __begin = __mutable_begin();
      ranges::inplace_merge(__begin, __it, __end, __value_comp_ref());
      __it = std::__flatmap_unique(__begin, __end, __value_comp_ref());
      __keys_.erase(__it.__key(), __end.__key());
      __values_.erase(__it.__value(), __end.__value());
    }
    __guard.__complete();
  }

  _LIBCPP_HIDE_FROM_ABI
  void insert(initializer_list<value_type> __il) { insert(__il.begin(), __il.end()); }

  _LIBCPP_HIDE_FROM_ABI
  void insert(sorted_unique_t __s, initializer_list<value_type> __il) { insert(__s, __il.begin(), __il.end()); }

  _LIBCPP_HIDE_FROM_ABI
  containers extract() && {
    auto __guard = std::__make_exception_guard([&] { clear(); });
    containers __res = { std::move(__keys_), std::move(__values_) };
    // deliberately do not call __guard.__complete(), so that the clear always happens
    return __res;
  }

  _LIBCPP_HIDE_FROM_ABI
  void replace(key_container_type&& __keys, mapped_container_type&& __values) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__keys.size() == __values.size(),
        "the number of keys must equal the number of values");
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    __keys_ = std::move(__keys);
    __values_ = std::move(__values);
    __guard.__complete();
    _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted_uniqued(begin(), end()),
        "replace expects input that is sorted and uniqued");
  }

  template <class... _Args>
    requires is_constructible_v<mapped_type, _Args...>
  _LIBCPP_HIDE_FROM_ABI
  pair<iterator, bool> try_emplace(const key_type& __k, _Args&&... __args) {
    auto __pos = lower_bound(__k);
    if (__pos == end() || __compare_(__k, *__pos.__key())) {
      return {__emplace_unchecked(__pos, __k, std::forward<_Args>(__args)...), true};
    } else {
      return {__pos, false};
    }
  }

  template <class... _Args>
    requires is_constructible_v<mapped_type, _Args...>
  _LIBCPP_HIDE_FROM_ABI
  pair<iterator, bool> try_emplace(key_type&& __k, _Args&&... __args) {
    auto __pos = lower_bound(__k);
    if (__pos == end() || __compare_(__k, *__pos.__key())) {
      return {__emplace_unchecked(__pos, std::move(__k), std::forward<_Args>(__args)...), true};
    } else {
      return {__pos, false};
    }
  }

  template <class _K2, class... _Args>
    requires __is_transparent_v<_Compare, _K2> &&
             is_constructible_v<key_type, _K2> &&
             is_constructible_v<mapped_type, _Args...> &&
             (!is_convertible_v<_K2&&, const_iterator>)
  _LIBCPP_HIDE_FROM_ABI
  pair<iterator, bool> try_emplace(_K2&& __k, _Args&&... __args) {
    auto __pos = lower_bound(__k);
    if (__pos == end() || __compare_(__k, *__pos.__key())) {
      return {__emplace_unchecked(__pos, std::forward<_K2>(__k), std::forward<_Args>(__args)...), true};
    } else {
      return {__pos, false};
    }
  }

  template <class... _Args>
    requires is_constructible_v<mapped_type, _Args...>
  _LIBCPP_HIDE_FROM_ABI
  iterator try_emplace(const_iterator __pos, const key_type& __k, _Args&&... __args) {
    auto __kt = __pos.__key();
    if (__kt == __keys_.end() || __comp_ref()(__k, *__kt)) {
      if (__kt == __keys_.begin() || __comp_ref()(__kt[-1], __k)) {
        return __emplace_unchecked(__pos, __k, std::forward<_Args>(__args)...);
      }
    }
    __pos = lower_bound(__k);
    if (__pos == end() || __compare_(__k, *__pos.__key())) {
      return __emplace_unchecked(__pos, __k, std::forward<_Args>(__args)...);
    } else {
      return __pos;
    }
  }

  template <class... _Args>
    requires is_constructible_v<mapped_type, _Args...>
  _LIBCPP_HIDE_FROM_ABI
  iterator try_emplace(const_iterator __pos, key_type&& __k, _Args&&... __args) {
    auto __kt = __pos.__key();
    if (__kt == __keys_.end() || __comp_ref()(__k, *__kt)) {
      if (__kt == __keys_.begin() || __comp_ref()(__kt[-1], __k)) {
        return __emplace_unchecked(__pos, std::move(__k), std::forward<_Args>(__args)...);
      }
    }
    __pos = lower_bound(__k);
    if (__pos == end() || __compare_(__k, *__pos.__key())) {
      return __emplace_unchecked(__pos, std::move(__k), std::forward<_Args>(__args)...);
    } else {
      return __pos;
    }
  }

  template <class _K2, class... _Args>
    requires __is_transparent_v<_Compare, _K2> &&
             is_constructible_v<key_type, _K2> &&
             is_constructible_v<mapped_type, _Args...> &&
             (!is_convertible_v<_K2&&, const_iterator>)
  _LIBCPP_HIDE_FROM_ABI
  iterator try_emplace(const_iterator __pos, _K2&& __k, _Args&&... __args) {
    auto __kt = __pos.__key();
    if (__kt == __keys_.end() || __comp_ref()(__k, *__kt)) {
      if (__kt == __keys_.begin() || __comp_ref()(__kt[-1], __k)) {
        return __emplace_unchecked(__pos, std::forward<_K2>(__k), std::forward<_Args>(__args)...);
      }
    }
    __pos = lower_bound(__k);
    if (__pos == end() || __compare_(__k, *__pos.__key())) {
      return __emplace_unchecked(__pos, std::forward<_K2>(__k), std::forward<_Args>(__args)...);
    } else {
      return __pos;
    }
  }

  template <class _M2>
    requires is_assignable_v<mapped_type&, _M2> &&
             is_constructible_v<mapped_type, _M2>
  pair<iterator, bool> insert_or_assign(const key_type& __k, _M2&& __obj) {
    auto __pos = lower_bound(__k);
    if (__pos == end() || __compare_(__k, *__pos.__key())) {
      return {__emplace_unchecked(__pos, __k, std::forward<_M2>(__obj)), true};
    } else {
      *__pos.__value() = std::forward<_M2>(__obj);
      return {__pos, false};
    }
  }

  template <class _M2>
    requires is_assignable_v<mapped_type&, _M2> &&
             is_constructible_v<mapped_type, _M2>
  pair<iterator, bool> insert_or_assign(key_type&& __k, _M2&& __obj) {
    auto __pos = lower_bound(__k);
    if (__pos == end() || __compare_(__k, *__pos.__key())) {
      return {__emplace_unchecked(__pos, std::move(__k), std::forward<_M2>(__obj)), true};
    } else {
      *__pos.__value() = std::forward<_M2>(__obj);
      return {__pos, false};
    }
  }

  template <class _K2, class _M2>
    requires __is_transparent_v<_Compare, _K2> &&
             is_constructible_v<key_type, _K2> &&
             is_assignable_v<mapped_type&, _M2> &&
             is_constructible_v<mapped_type, _M2>
  pair<iterator, bool> insert_or_assign(_K2&& __k, _M2&& __obj) {
    auto __pos = lower_bound(__k);
    if (__pos == end() || __compare_(__k, *__pos.__key())) {
      return {__emplace_unchecked(__pos, std::forward<_K2>(__k), std::forward<_M2>(__obj)), true};
    } else {
      *__pos.__value() = std::forward<_M2>(__obj);
      return {__pos, false};
    }
  }

  template <class _M2>
    requires is_assignable_v<mapped_type&, _M2> &&
             is_constructible_v<mapped_type, _M2>
  iterator insert_or_assign(const_iterator __pos, const key_type& __k, _M2&& __obj) {
    auto __kt = __pos.__key();
    if (__kt == __keys_.end() || __compare_(__k, *__kt)) {
      if (__kt == __keys_.begin() || __compare_(__kt[-1], __k)) {
        return __emplace_unchecked(__pos, __k, std::forward<_M2>(__obj));
      }
    } else if (!__compare_(*__kt, __k)) {
      *__pos.__value() = std::forward<_M2>(__obj);
      return __pos;
    }
    return insert_or_assign(__k, std::forward<_M2>(__obj));
  }

  template <class _M2>
    requires is_assignable_v<mapped_type&, _M2> &&
             is_constructible_v<mapped_type, _M2>
  iterator insert_or_assign(const_iterator __pos, key_type&& __k, _M2&& __obj) {
    auto __kt = __pos.__key();
    if (__kt == __keys_.end() || __compare_(__k, *__kt)) {
      if (__kt == __keys_.begin() || __compare_(__kt[-1], __k)) {
        return __emplace_unchecked(__pos, std::move(__k), std::forward<_M2>(__obj));
      }
    } else if (!__compare_(*__kt, __k)) {
      *__pos.__value() = std::forward<_M2>(__obj);
      return __pos;
    }
    return insert_or_assign(std::move(__k), std::forward<_M2>(__obj));
  }

  template <class _K2, class _M2>
    requires __is_transparent_v<_Compare, _K2> &&
             is_constructible_v<key_type, _K2> &&
             is_assignable_v<mapped_type&, _M2> &&
             is_constructible_v<mapped_type, _M2>
  iterator insert_or_assign(const_iterator __pos, _K2&& __k, _M2&& __obj) {
    auto __kt = __pos.__key();
    if (__kt == __keys_.end() || __compare_(__k, *__kt)) {
      if (__kt == __keys_.begin() || __compare_(__kt[-1], __k)) {
        return __emplace_unchecked(__pos, std::forward<_K2>(__k), std::forward<_M2>(__obj));
      }
    } else if (!__compare_(*__kt, __k)) {
      *__pos.__value() = std::forward<_M2>(__obj);
      return __pos;
    }
    return insert_or_assign(std::forward<_K2>(__k), std::forward<_M2>(__obj));
  }

  _LIBCPP_HIDE_FROM_ABI
  iterator erase(iterator __pos) {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    auto __kt = __keys_.erase(__pos.__key());
    auto __vt = __values_.erase(__pos.__value());
    __guard.__complete();
    return iterator(__kt, __vt);
  }

  _LIBCPP_HIDE_FROM_ABI
  iterator erase(const_iterator __pos) {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    auto __kt = __keys_.erase(__pos.__key());
    auto __vt = __values_.erase(__pos.__value());
    __guard.__complete();
    return iterator(__kt, __vt);
  }

  _LIBCPP_HIDE_FROM_ABI
  size_type erase(const key_type& __x) {
    auto __it = find(__x);
    if (__it != end()) {
      erase(__it);
      return 1;
    }
    return 0;
  }

  template <class _K2>
    requires __is_transparent_v<_Compare, _K2> &&
             (!is_convertible_v<_K2&&, iterator>) &&
             (!is_convertible_v<_K2&&, const_iterator>)
  _LIBCPP_HIDE_FROM_ABI
  size_type erase(_K2&& __x) {
    auto [__first, __last] = equal_range(std::forward<_K2>(__x));
    auto __n = __last - __first;
    if (__n) {
      erase(__first, __last);
    }
    return __n;
  }

  _LIBCPP_HIDE_FROM_ABI
  iterator erase(const_iterator __first, const_iterator __last) {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    auto __kt = __keys_.erase(__first.__key(), __last.__key());
    auto __vt = __values_.erase(__first.__value(), __last.__value());
    __guard.__complete();
    return iterator(__kt, __vt);
  }

  _LIBCPP_HIDE_FROM_ABI
  void swap(flat_map& __y) noexcept {
    using std::swap;
    swap(__compare_, __y.__compare_);
    swap(__keys_, __y.__keys_);
    swap(__values_, __y.__values_);
  }

  _LIBCPP_HIDE_FROM_ABI
  void clear() noexcept {
    __keys_.clear();
    __values_.clear();
  }

  // observers

  _LIBCPP_HIDE_FROM_ABI key_compare key_comp() const { return __compare_; }
  _LIBCPP_HIDE_FROM_ABI value_compare value_comp() const { return value_compare(__compare_); }

  _LIBCPP_HIDE_FROM_ABI const key_container_type& keys() const noexcept { return __keys_; }
  _LIBCPP_HIDE_FROM_ABI const mapped_container_type& values() const noexcept { return __values_; }

  // map operations

  _LIBCPP_HIDE_FROM_ABI
  iterator find(const key_type& __x) {
    auto __it = lower_bound(__x);
    if (__it == end() || __compare_(__x, *__it.__key())) {
      return end();
    }
    return __it;
  }

  _LIBCPP_HIDE_FROM_ABI
  const_iterator find(const key_type& __x) const {
    auto __it = lower_bound(__x);
    if (__it == end() || __compare_(__x, *__it.__key())) {
      return end();
    }
    return __it;
  }

  template <class _K2>
    requires __is_transparent_v<_Compare, _K2>
  _LIBCPP_HIDE_FROM_ABI
  iterator find(const _K2& __x) {
    auto __it = lower_bound(__x);
    if (__it == end() || __compare_(__x, *__it.__key())) {
      return end();
    }
    return __it;
  }

  template <class _K2>
    requires __is_transparent_v<_Compare, _K2>
  _LIBCPP_HIDE_FROM_ABI
  const_iterator find(const _K2& __x) const {
    auto __it = lower_bound(__x);
    if (__it == end() || __compare_(__x, *__it.__key())) {
      return end();
    }
    return __it;
  }

  _LIBCPP_HIDE_FROM_ABI
  size_type count(const key_type& __x) const {
    return contains(__x) ? 1 : 0;
  }

  template <class _K2>
    requires __is_transparent_v<_Compare, _K2>
  _LIBCPP_HIDE_FROM_ABI
  size_type count(const _K2& __x) const {
    auto [__first, __last] = equal_range(__x);
    return __last - __first;
  }

  _LIBCPP_HIDE_FROM_ABI
  bool contains(const key_type& __x) const {
    return find(__x) != end();
  }

  template <class _K2>
    requires __is_transparent_v<_Compare, _K2>
  _LIBCPP_HIDE_FROM_ABI
  bool contains(const _K2& __x) const {
    return find(__x) != end();
  }

  _LIBCPP_HIDE_FROM_ABI
  iterator lower_bound(const key_type& __x) {
    auto __kt = std::partition_point(__keys_.begin(), __keys_.end(), [&](auto&& __e) {
      return __compare_(decltype(__e)(__e), __x);
    });
    return iterator(__kt, __values_.begin() + (__kt - __keys_.begin()));
  }

  _LIBCPP_HIDE_FROM_ABI
  const_iterator lower_bound(const key_type& __x) const {
    auto __kt = std::partition_point(__keys_.begin(), __keys_.end(), [&](auto&& __e) {
      return __compare_(decltype(__e)(__e), __x);
    });
    return const_iterator(__kt, __values_.begin() + (__kt - __keys_.begin()));
  }

  template <class _K2>
    requires __is_transparent_v<_Compare, _K2>
  _LIBCPP_HIDE_FROM_ABI
  iterator lower_bound(const _K2& __x) {
    auto __kt = std::partition_point(__keys_.begin(), __keys_.end(), [&](auto&& __e) {
      return __compare_(decltype(__e)(__e), __x);
    });
    return iterator(__kt, __values_.begin() + (__kt - __keys_.begin()));
  }

  template <class _K2>
    requires __is_transparent_v<_Compare, _K2>
  _LIBCPP_HIDE_FROM_ABI
  const_iterator lower_bound(const _K2& __x) const {
    auto __kt = std::partition_point(__keys_.begin(), __keys_.end(), [&](auto&& __e) {
      return __compare_(decltype(__e)(__e), __x);
    });
    return const_iterator(__kt, __values_.begin() + (__kt - __keys_.begin()));
  }

  _LIBCPP_HIDE_FROM_ABI
  iterator upper_bound(const key_type& __x) {
    auto __kt = std::partition_point(__keys_.begin(), __keys_.end(), [&](auto&& __e) {
      return !__compare_(__x, decltype(__e)(__e));
    });
    return iterator(__kt, __values_.begin() + (__kt - __keys_.begin()));
  }

  _LIBCPP_HIDE_FROM_ABI
  const_iterator upper_bound(const key_type& __x) const {
    auto __kt = std::partition_point(__keys_.begin(), __keys_.end(), [&](auto&& __e) {
      return !__compare_(__x, decltype(__e)(__e));
    });
    return const_iterator(__kt, __values_.begin() + (__kt - __keys_.begin()));
  }

  template <class _K2>
    requires __is_transparent_v<_Compare, _K2>
  _LIBCPP_HIDE_FROM_ABI
  iterator upper_bound(const _K2& __x) {
    auto __kt = std::partition_point(__keys_.begin(), __keys_.end(), [&](auto&& __e) {
      return !__compare_(__x, decltype(__e)(__e));
    });
    return iterator(__kt, __values_.begin() + (__kt - __keys_.begin()));
  }

  template <class _K2>
    requires __is_transparent_v<_Compare, _K2>
  _LIBCPP_HIDE_FROM_ABI
  const_iterator upper_bound(const _K2& __x) const {
    auto __kt = std::partition_point(__keys_.begin(), __keys_.end(), [&](auto&& __e) {
      return !__compare_(__x, decltype(__e)(__e));
    });
    return const_iterator(__kt, __values_.begin() + (__kt - __keys_.begin()));
  }

  _LIBCPP_HIDE_FROM_ABI
  pair<iterator, iterator> equal_range(const key_type& __x) {
    auto __kt = std::partition_point(__keys_.begin(), __keys_.end(), [&](auto&& __e) {
      return __compare_(decltype(__e)(__e), __x);
    });
    auto __first = iterator(__kt, __values_.begin() + (__kt - __keys_.begin()));
    if (__kt == __keys_.end() || __compare_(__x, *__kt)) {
      return {__first, __first};
    }
    return {__first, __first + 1};
  }

  _LIBCPP_HIDE_FROM_ABI
  pair<const_iterator, const_iterator> equal_range(const key_type& __x) const {
    auto __kt = std::partition_point(__keys_.begin(), __keys_.end(), [&](auto&& __e) {
      return __compare_(decltype(__e)(__e), __x);
    });
    auto __first = const_iterator(__kt, __values_.begin() + (__kt - __keys_.begin()));
    if (__kt == __keys_.end() || __compare_(__x, *__kt)) {
      return {__first, __first};
    }
    return {__first, __first + 1};
  }

  template <class _K2>
    requires __is_transparent_v<_Compare, _K2>
  _LIBCPP_HIDE_FROM_ABI
  pair<iterator, iterator> equal_range(const _K2& __x) {
    auto __kend = __keys_.end();
    auto __kfirst = std::partition_point(__keys_.begin(), __kend, [&](const key_type& __e) {
      return __compare_(__e, __x);
    });
    auto __klast = std::partition_point(__kfirst, __kend, [&](const key_type& __e) {
      return !__compare_(__x, __e);
    });
    auto __first = iterator(__kfirst, __values_.begin() + (__kfirst - __keys_.begin()));
    auto __last = iterator(__klast, __values_.begin() + (__klast - __keys_.begin()));
    return {__first, __last};
  }

  template <class _K2>
    requires __is_transparent_v<_Compare, _K2>
  _LIBCPP_HIDE_FROM_ABI
  pair<const_iterator, const_iterator> equal_range(const _K2& __x) const {
    auto __kend = __keys_.end();
    auto __kfirst = std::partition_point(__keys_.begin(), __kend, [&](const key_type& __e) {
      return __compare_(__e, __x);
    });
    auto __klast = std::partition_point(__kfirst, __kend, [&](const key_type& __e) {
      return !__compare_(__x, __e);
    });
    auto __first = const_iterator(__kfirst, __values_.begin() + (__kfirst - __keys_.begin()));
    auto __last = const_iterator(__klast, __values_.begin() + (__klast - __keys_.begin()));
    return {__first, __last};
  }

  _LIBCPP_HIDE_FROM_ABI
  friend bool operator==(const flat_map& __x, const flat_map& __y) {
    return (__x.__keys_ == __y.__keys_) && (__x.__values_ == __y.__values_);
  }

  _LIBCPP_HIDE_FROM_ABI
  friend auto operator<=>(const flat_map& __x, const flat_map& __y)
    requires requires (const_iterator __it) { __synth_three_way(*__it, *__it); }
  {
    return std::lexicographical_compare_three_way(__x.begin(), __x.end(), __y.begin(), __y.end(), __synth_three_way<const_reference, const_reference>);
  }

  _LIBCPP_HIDE_FROM_ABI
  friend void swap(flat_map& __x, flat_map& __y) noexcept {
    __x.swap(__y);
  }

  template <class _Predicate>
  _LIBCPP_HIDE_FROM_ABI
  size_type __erase_if(reference_wrapper<const _Predicate> __pred) {
    auto __begin = __mutable_begin();
    auto __end = __mutable_end();
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    auto __it = std::remove_if(__begin, __end, [&](const_reference __e) -> bool {
      // `remove_if` will hand us `pair<K&, V&>`, but we'll hand `pair<const K&, const V&>` to the predicate.
      return __pred(__e);
    });
    auto __n = __end - __it;
    __keys_.erase(__it.__key(), __end.__key());
    __values_.erase(__it.__value(), __end.__value());
    __guard.__complete();
    return __n;
  }

private:
  _LIBCPP_HIDE_FROM_ABI
  void __construct_invariant() {
     // This function must not be called except from the constructors,
     // where if the comparator, move-assignment, or erasure throws an exception
     // we will have no flat_map object and thus no invariant to restore.
     auto __begin = __mutable_begin();
     auto __end = __mutable_end();
     ranges::stable_sort(__begin, __end, __value_comp_ref());
     auto __it = std::__flatmap_unique(__begin, __end, __value_comp_ref());
     __keys_.erase(__it.__key(), __keys_.end());
     __values_.erase(__it.__value(), __values_.end());
  }

  _LIBCPP_HIDE_FROM_ABI
  void __restore_invariant() {
    // There is already an exception in flight, so we cannot do anything
    // that would risk a second exception. In particular, the comparator
    // might throw, or either component type's operator= might throw.
    // All we can safely do, in general, is clear the containers.
    __keys_.clear();
    __values_.clear();
  }

  _LIBCPP_HIDE_FROM_ABI
  bool __is_sorted_uniqued(const_iterator __first, const_iterator __last) const {
    auto __not_less = [&](const auto& __a, const auto& __b) { return !__compare_(__a, __b); };
    return std::adjacent_find(__first.__key(), __last.__key(), __not_less) == __last.__key();
  }

  _LIBCPP_HIDE_FROM_ABI
  reference_wrapper<const _Compare> __comp_ref() const {
    return reference_wrapper<const _Compare>(__compare_);
  }

  _LIBCPP_HIDE_FROM_ABI
  __flatmap_value_comp_ref<_Compare> __value_comp_ref() const {
    return __flatmap_value_comp_ref<_Compare>(__compare_);
  }

  _LIBCPP_HIDE_FROM_ABI
  __mutable_iterator __mutable_begin() {
    return __mutable_iterator(__keys_.begin(), __values_.begin());
  }

  _LIBCPP_HIDE_FROM_ABI
  __mutable_iterator __mutable_end() {
    return __mutable_iterator(__keys_.end(), __values_.end());
  }

  template <class _InputIterator, class _Sentinel>
  _LIBCPP_HIDE_FROM_ABI
  size_type __append_pairs(_InputIterator __first, _Sentinel __last) {
    size_type __n = 0;
    for ( ; __first != __last; ++__first) {
      value_type __kv = *__first;
      __keys_.insert(__keys_.end(), std::move(__kv.first));
      __values_.insert(__values_.end(), std::move(__kv.second));
      ++__n;
    }
    return __n;
  }

  template <class _K2>
  _LIBCPP_HIDE_FROM_ABI
  iterator __insert_unchecked(const_iterator __pos, _K2&& __x) {
    // Insert __x into the underlying container at __pos, which is assumed
    // to be the correct position for that element. That is, a successful
    // insertion preserves the container invariant.
    //
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    auto __kt = __keys_.insert(__pos.__key(), std::forward<_K2>(__x).first);
    auto __vt = __values_.insert(__pos.__value(), std::forward<_K2>(__x).second);
    __guard.__complete();
    return iterator(__kt, __vt);
  }

  template <class _K2, class... _Args>
  _LIBCPP_HIDE_FROM_ABI
  iterator __emplace_unchecked(const_iterator __pos, _K2&& __k, _Args&&... __args) {
    // Emplace {__x, mapped_type(__args...)} into the underlying containers at __pos,
    // which is assumed to be the correct position for that element. That is,
    // a successful insertion preserves the container invariant.
    //
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    auto __kt = __keys_.emplace(__pos.__key(), std::forward<_K2>(__k));
    auto __vt = __values_.emplace(__pos.__value(), std::forward<_Args>(__args)...);
    __guard.__complete();
    return iterator(__kt, __vt);
  }

  _LIBCPP_NO_UNIQUE_ADDRESS key_compare __compare_ = key_compare();
  key_container_type __keys_ = key_container_type();
  mapped_container_type __values_ = mapped_container_type();
};

template<class _KeyContainer,
         class _MappedContainer,
         class _Compare = less<typename _KeyContainer::value_type>,
         class = typename _KeyContainer::const_iterator,
         class = typename _MappedContainer::const_iterator,
         class = enable_if_t<!__is_allocator<_Compare>::value>>
flat_map(_KeyContainer, _MappedContainer, _Compare = _Compare())
  -> flat_map<typename _KeyContainer::value_type, typename _MappedContainer::value_type,
              _Compare, _KeyContainer, _MappedContainer>;

template<class _KeyContainer,
         class _MappedContainer,
         class _Compare,
         class _Alloc,
         class = typename _KeyContainer::const_iterator,
         class = typename _MappedContainer::const_iterator,
         class = enable_if_t<!__is_allocator<_Compare>::value>,
         class = enable_if_t<uses_allocator_v<_KeyContainer, _Alloc> && uses_allocator_v<_MappedContainer, _Alloc>>>
flat_map(_KeyContainer, _MappedContainer, _Compare, _Alloc)
  -> flat_map<typename _KeyContainer::value_type, typename _MappedContainer::value_type,
              _Compare, _KeyContainer, _MappedContainer>;

template<class _KeyContainer,
         class _MappedContainer,
         class _Alloc,
         class = typename _KeyContainer::const_iterator,
         class = typename _MappedContainer::const_iterator,
         class = enable_if_t<uses_allocator_v<_KeyContainer, _Alloc> && uses_allocator_v<_MappedContainer, _Alloc>>>
  requires true
flat_map(_KeyContainer, _MappedContainer, _Alloc)
  -> flat_map<typename _KeyContainer::value_type, typename _MappedContainer::value_type,
              less<typename _KeyContainer::value_type>, _KeyContainer, _MappedContainer>;

template<class _KeyContainer,
         class _MappedContainer,
         class _Compare = less<typename _KeyContainer::value_type>,
         class = typename _KeyContainer::const_iterator,
         class = typename _MappedContainer::const_iterator,
         class = enable_if_t<!__is_allocator<_Compare>::value>>
flat_map(sorted_unique_t, _KeyContainer, _MappedContainer, _Compare = _Compare())
  -> flat_map<typename _KeyContainer::value_type, typename _MappedContainer::value_type,
              _Compare, _KeyContainer, _MappedContainer>;

template<class _KeyContainer,
         class _MappedContainer,
         class _Compare,
         class _Alloc,
         class = typename _KeyContainer::const_iterator,
         class = typename _MappedContainer::const_iterator,
         class = enable_if_t<!__is_allocator<_Compare>::value>,
         class = enable_if_t<uses_allocator_v<_KeyContainer, _Alloc> && uses_allocator_v<_MappedContainer, _Alloc>>>
flat_map(sorted_unique_t, _KeyContainer, _MappedContainer, _Compare, _Alloc)
  -> flat_map<typename _KeyContainer::value_type, typename _MappedContainer::value_type,
              _Compare, _KeyContainer, _MappedContainer>;

template<class _KeyContainer,
         class _MappedContainer,
         class _Alloc,
         class = typename _KeyContainer::const_iterator,
         class = typename _MappedContainer::const_iterator,
         class = enable_if_t<uses_allocator_v<_KeyContainer, _Alloc> && uses_allocator_v<_MappedContainer, _Alloc>>>
  requires true
flat_map(sorted_unique_t, _KeyContainer, _MappedContainer, _Alloc)
  -> flat_map<typename _KeyContainer::value_type, typename _MappedContainer::value_type,
              less<typename _KeyContainer::value_type>, _KeyContainer, _MappedContainer>;

template<class _InputIterator,
         class _Compare = less<__iter_key_type<_InputIterator>>,
         class _Allocator = allocator<int>,
         class = enable_if_t<__has_input_iterator_category<_InputIterator>::value>,
         class = enable_if_t<!__is_allocator<_Compare>::value>,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_map(_InputIterator, _InputIterator, _Compare = _Compare(), _Allocator = _Allocator())
  -> flat_map<__iter_key_type<_InputIterator>, __iter_mapped_type<_InputIterator>, _Compare,
              vector<__iter_key_type<_InputIterator>, __allocator_traits_rebind_t<_Allocator, __iter_key_type<_InputIterator>>>,
              vector<__iter_mapped_type<_InputIterator>, __allocator_traits_rebind_t<_Allocator, __iter_mapped_type<_InputIterator>>>>;

template<class _InputIterator,
         class _Compare = less<__iter_key_type<_InputIterator>>,
         class _Allocator = allocator<int>,
         class = enable_if_t<__has_input_iterator_category<_InputIterator>::value>,
         class = enable_if_t<!__is_allocator<_Compare>::value>,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_map(sorted_unique_t, _InputIterator, _InputIterator, _Compare = _Compare(), _Allocator = _Allocator())
  -> flat_map<__iter_key_type<_InputIterator>, __iter_mapped_type<_InputIterator>, _Compare,
              vector<__iter_key_type<_InputIterator>, __allocator_traits_rebind_t<_Allocator, __iter_key_type<_InputIterator>>>,
              vector<__iter_mapped_type<_InputIterator>, __allocator_traits_rebind_t<_Allocator, __iter_mapped_type<_InputIterator>>>>;

template<class _InputIterator,
         class _Allocator,
         class = enable_if_t<__has_input_iterator_category<_InputIterator>::value>,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_map(_InputIterator, _InputIterator, _Allocator)
  -> flat_map<__iter_key_type<_InputIterator>, __iter_mapped_type<_InputIterator>, less<__iter_key_type<_InputIterator>>,
              vector<__iter_key_type<_InputIterator>, __allocator_traits_rebind_t<_Allocator, __iter_key_type<_InputIterator>>>,
              vector<__iter_mapped_type<_InputIterator>, __allocator_traits_rebind_t<_Allocator, __iter_mapped_type<_InputIterator>>>>;

template<class _InputIterator,
         class _Allocator = allocator<int>,
         class = enable_if_t<__has_input_iterator_category<_InputIterator>::value>,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_map(sorted_unique_t, _InputIterator, _InputIterator, _Allocator)
  -> flat_map<__iter_key_type<_InputIterator>, __iter_mapped_type<_InputIterator>, less<__iter_key_type<_InputIterator>>,
              vector<__iter_key_type<_InputIterator>, __allocator_traits_rebind_t<_Allocator, __iter_key_type<_InputIterator>>>,
              vector<__iter_mapped_type<_InputIterator>, __allocator_traits_rebind_t<_Allocator, __iter_mapped_type<_InputIterator>>>>;

template<ranges::input_range _Rp,
         class _Compare = less<__range_key_type<_Rp>>,
         class _Allocator = allocator<int>,
         class = enable_if_t<!__is_allocator<_Compare>::value>,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_map(from_range_t, _Rp&&, _Compare = _Compare(), _Allocator = _Allocator())
  -> flat_map<__range_key_type<_Rp>, __range_mapped_type<_Rp>, _Compare,
              vector<__range_key_type<_Rp>, __allocator_traits_rebind_t<_Allocator, __range_key_type<_Rp>>>,
              vector<__range_mapped_type<_Rp>, __allocator_traits_rebind_t<_Allocator, __range_mapped_type<_Rp>>>>;

template<ranges::input_range _Rp,
         class _Allocator,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_map(from_range_t, _Rp&&, _Allocator)
  -> flat_map<__range_key_type<_Rp>, __range_mapped_type<_Rp>, less<__range_key_type<_Rp>>,
              vector<__range_key_type<_Rp>, __allocator_traits_rebind_t<_Allocator, __range_key_type<_Rp>>>,
              vector<__range_mapped_type<_Rp>, __allocator_traits_rebind_t<_Allocator, __range_mapped_type<_Rp>>>>;

template<class _Key, class _Tp,
         class _Compare = less<_Key>,
         class _Allocator = allocator<_Key>,
         class = enable_if_t<!__is_allocator<_Compare>::value>,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_map(initializer_list<pair<_Key, _Tp>>, _Compare = _Compare(), _Allocator = _Allocator())
  -> flat_map<_Key, _Tp, _Compare,
              vector<_Key, __allocator_traits_rebind_t<_Allocator, _Key>>,
              vector<_Tp, __allocator_traits_rebind_t<_Allocator, _Tp>>>;

template<class _Key, class _Tp,
         class _Compare = less<_Key>,
         class _Allocator = allocator<_Key>,
         class = enable_if_t<!__is_allocator<_Compare>::value>,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_map(sorted_unique_t, initializer_list<pair<_Key, _Tp>>, _Compare = _Compare(), _Allocator = _Allocator())
  -> flat_map<_Key, _Tp, _Compare,
              vector<_Key, __allocator_traits_rebind_t<_Allocator, _Key>>,
              vector<_Tp, __allocator_traits_rebind_t<_Allocator, _Tp>>>;

template<class _Key, class _Tp,
         class _Allocator,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_map(initializer_list<pair<_Key, _Tp>>, _Allocator)
  -> flat_map<_Key, _Tp, less<_Key>,
              vector<_Key, __allocator_traits_rebind_t<_Allocator, _Key>>,
              vector<_Tp, __allocator_traits_rebind_t<_Allocator, _Tp>>>;

template<class _Key, class _Tp,
         class _Allocator,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_map(sorted_unique_t, initializer_list<pair<_Key, _Tp>>, _Allocator)
  -> flat_map<_Key, _Tp, less<_Key>,
              vector<_Key, __allocator_traits_rebind_t<_Allocator, _Key>>,
              vector<_Tp, __allocator_traits_rebind_t<_Allocator, _Tp>>>;

template <class _Key, class _Tp, class _Compare, class _KeyContainer, class _MappedContainer, class _Alloc>
struct uses_allocator<flat_map<_Key, _Tp, _Compare, _KeyContainer, _MappedContainer>, _Alloc>
  : bool_constant<uses_allocator_v<_KeyContainer, _Alloc> &&
                  uses_allocator_v<_MappedContainer, _Alloc>> { };

// [flat.map.erasure]

template <class _Key, class _Tp, class _Compare, class _KeyContainer, class _MappedContainer, class _Predicate>
_LIBCPP_HIDE_FROM_ABI
size_t erase_if(flat_map<_Key, _Tp, _Compare, _KeyContainer, _MappedContainer>& __c, _Predicate __pred)
{
    return __c.__erase_if(reference_wrapper<const _Predicate>(__pred));
}

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP_STD_VER >= 23

_LIBCPP_POP_MACROS

#endif // _LIBCPP___FLAT_SET_FLAT_MAP_H
