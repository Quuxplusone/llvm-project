// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___FLAT_SET_FLAT_MULTISET_H
#define _LIBCPP___FLAT_SET_FLAT_MULTISET_H

/*

namespace std {
  template<class Key, class Compare = less<Key>, class KeyContainer = vector<Key>>
  class flat_multiset {
  public:
    // types
    using key_type                  = Key;
    using value_type                = Key;
    using key_compare               = Compare;
    using value_compare             = Compare;
    using reference                 = value_type&;
    using const_reference           = const value_type&;
    using size_type                 = typename KeyContainer::size_type;
    using difference_type           = typename KeyContainer::difference_type;
    using iterator                  = implementation-defined;  // see [container.requirements]
    using const_iterator            = implementation-defined;  // see [container.requirements]
    using reverse_iterator          = std::reverse_iterator<iterator>;
    using const_reverse_iterator    = std::reverse_iterator<const_iterator>;
    using container_type            = KeyContainer;

    // [flat.multiset.cons], constructors
    flat_multiset() : flat_multiset(key_compare()) { }

    explicit flat_multiset(container_type cont, const key_compare& comp = key_compare());
    template<class Allocator>
      flat_multiset(const container_type& cont, const Allocator& a);
    template<class Allocator>
      flat_multiset(const container_type& cont, const key_compare& comp, const Allocator& a);

    flat_multiset(sorted_equivalent_t, container_type cont,
                  const key_compare& comp = key_compare())
      : c(std::move(cont)), compare(comp) { }
    template<class Allocator>
      flat_multiset(sorted_equivalent_t, const container_type&, const Allocator& a);
    template<class Allocator>
      flat_multiset(sorted_equivalent_t, const container_type& cont,
                    const key_compare& comp, const Allocator& a);

    explicit flat_multiset(const key_compare& comp)
      : c(), compare(comp) { }
    template<class Allocator>
      flat_multiset(const key_compare& comp, const Allocator& a);
    template<class Allocator>
      explicit flat_multiset(const Allocator& a);

    template<class InputIterator>
      flat_multiset(InputIterator first, InputIterator last,
                    const key_compare& comp = key_compare())
        : c(), compare(comp)
        { insert(first, last); }
    template<class InputIterator, class Allocator>
      flat_multiset(InputIterator first, InputIterator last,
                    const key_compare& comp, const Allocator& a);
    template<class InputIterator, class Allocator>
      flat_multiset(InputIterator first, InputIterator last, const Allocator& a);

    template<container-compatible-range<value_type> R>
      flat_multiset(from_range_t fr, R&& rg)
        : flat_multiset(fr, std::forward<R>(rg), key_compare()) { }
    template<container-compatible-range<value_type> R, class Allocator>
      flat_multiset(from_range_t, R&& rg, const Allocator& a);
    template<container-compatible-range<value_type> R>
      flat_multiset(from_range_t, R&& rg, const key_compare& comp)
        : flat_multiset(comp)
        { insert_range(std::forward<R>(rg)); }
    template<container-compatible-range<value_type> R, class Allocator>
      flat_multiset(from_range_t, R&& rg, const key_compare& comp, const Allocator& a);

    template<class InputIterator>
      flat_multiset(sorted_equivalent_t, InputIterator first, InputIterator last,
                    const key_compare& comp = key_compare())
        : c(first, last), compare(comp) { }
    template<class InputIterator, class Allocator>
      flat_multiset(sorted_equivalent_t, InputIterator first, InputIterator last,
                    const key_compare& comp, const Allocator& a);
    template<class InputIterator, class Allocator>
      flat_multiset(sorted_equivalent_t, InputIterator first, InputIterator last,
                    const Allocator& a);

    flat_multiset(initializer_list<value_type> il, const key_compare& comp = key_compare())
      : flat_multiset(il.begin(), il.end(), comp) { }
    template<class Allocator>
      flat_multiset(initializer_list<value_type> il, const key_compare& comp,
                    const Allocator& a);
    template<class Allocator>
      flat_multiset(initializer_list<value_type> il, const Allocator& a);

    flat_multiset(sorted_equivalent_t s, initializer_list<value_type> il,
                  const key_compare& comp = key_compare())
        : flat_multiset(s, il.begin(), il.end(), comp) { }
    template<class Allocator>
      flat_multiset(sorted_equivalent_t, initializer_list<value_type> il,
                    const key_compare& comp, const Allocator& a);
    template<class Allocator>
      flat_multiset(sorted_equivalent_t, initializer_list<value_type> il, const Allocator& a);

    flat_multiset& operator=(initializer_list<value_type>);

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

    // capacity
    [[nodiscard]] bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;

    // [flat.multiset.modifiers], modifiers
    template<class... Args> iterator emplace(Args&&... args);
    template<class... Args>
      iterator emplace_hint(const_iterator position, Args&&... args);

    iterator insert(const value_type& x)
      { return emplace(x); }
    iterator insert(value_type&& x)
      { return emplace(std::move(x)); }
    iterator insert(const_iterator position, const value_type& x)
      { return emplace_hint(position, x); }
    iterator insert(const_iterator position, value_type&& x)
      { return emplace_hint(position, std::move(x)); }

    template<class InputIterator>
      void insert(InputIterator first, InputIterator last);
    template<class InputIterator>
      void insert(sorted_equivalent_t, InputIterator first, InputIterator last);
    template<container-compatible-range<value_type> R>
      void insert_range(R&& rg);

    void insert(initializer_list<value_type> il)
      { insert(il.begin(), il.end()); }
    void insert(sorted_equivalent_t s, initializer_list<value_type> il)
      { insert(s, il.begin(), il.end()); }

    container_type extract() &&;
    void replace(container_type&&);

    iterator erase(iterator position);
    iterator erase(const_iterator position);
    size_type erase(const key_type& x);
    template<class K> size_type erase(K&& x);
    iterator erase(const_iterator first, const_iterator last);

    void swap(flat_multiset& y) noexcept;
    void clear() noexcept;

    // observers
    key_compare key_comp() const;
    value_compare value_comp() const;

    // set operations
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
    template<class K>
      pair<iterator, iterator> equal_range(const K& x);
    template<class K>
      pair<const_iterator, const_iterator> equal_range(const K& x) const;

    friend bool operator==(const flat_multiset& x, const flat_multiset& y);

    friend synth-three-way-result<value_type>
      operator<=>(const flat_multiset& x, const flat_multiset& y);

    friend void swap(flat_multiset& x, flat_multiset& y) noexcept
      { x.swap(y); }

  private:
    container_type c;       // exposition only
    key_compare compare;    // exposition only
  };

  template<class KeyContainer, class Compare = less<typename KeyContainer::value_type>>
    flat_multiset(KeyContainer, Compare = Compare())
      -> flat_multiset<typename KeyContainer::value_type, Compare, KeyContainer>;
  template<class KeyContainer, class Allocator>
    flat_multiset(KeyContainer, Allocator)
      -> flat_multiset<typename KeyContainer::value_type,
                       less<typename KeyContainer::value_type>, KeyContainer>;
  template<class KeyContainer, class Compare, class Allocator>
    flat_multiset(KeyContainer, Compare, Allocator)
      -> flat_multiset<typename KeyContainer::value_type, Compare, KeyContainer>;

  template<class KeyContainer, class Compare = less<typename KeyContainer::value_type>>
    flat_multiset(sorted_equivalent_t, KeyContainer, Compare = Compare())
      -> flat_multiset<typename KeyContainer::value_type, Compare, KeyContainer>;
  template<class KeyContainer, class Allocator>
    flat_multiset(sorted_equivalent_t, KeyContainer, Allocator)
      -> flat_multiset<typename KeyContainer::value_type,
                       less<typename KeyContainer::value_type>, KeyContainer>;
  template<class KeyContainer, class Compare, class Allocator>
    flat_multiset(sorted_equivalent_t, KeyContainer, Compare, Allocator)
      -> flat_multiset<typename KeyContainer::value_type, Compare, KeyContainer>;

  template<class InputIterator, class Compare = less<iter-value-type<InputIterator>>>
    flat_multiset(InputIterator, InputIterator, Compare = Compare())
      -> flat_multiset<iter-value-type<InputIterator>, iter-value-type<InputIterator>, Compare>;

  template<class InputIterator, class Compare = less<iter-value-type<InputIterator>>>
    flat_multiset(sorted_equivalent_t, InputIterator, InputIterator, Compare = Compare())
      -> flat_multiset<iter-value-type<InputIterator>, iter-value-type<InputIterator>, Compare>;

  template<ranges::input_­range R, class Compare = less<ranges::range_value_t<R>>,
           class Allocator = allocator<ranges::range_value_t<R>>>
    flat_multiset(from_range_t, R&&, Compare = Compare(), Allocator = Allocator())
      -> flat_multiset<ranges::range_value_t<R>, Compare,
                       vector<ranges::range_value_t<R>,
                              alloc-rebind<Allocator, ranges::range_value_t<R>>>>;

  template<ranges::input_­range R, class Allocator>
    flat_multiset(from_range_t, R&&, Allocator)
      -> flat_multiset<ranges::range_value_t<R>, less<ranges::range_value_t<R>>,
                       vector<ranges::range_value_t<R>,
                              alloc-rebind<Allocator, ranges::range_value_t<R>>>>;

  template<class Key, class Compare = less<Key>>
    flat_multiset(initializer_list<Key>, Compare = Compare())
      -> flat_multiset<Key, Compare>;

  template<class Key, class Compare = less<Key>>
  flat_multiset(sorted_equivalent_t, initializer_list<Key>, Compare = Compare())
      -> flat_multiset<Key, Compare>;

  template<class Key, class Compare, class KeyContainer, class Allocator>
    struct uses_allocator<flat_multiset<Key, Compare, KeyContainer>, Allocator>
      : bool_constant<uses_allocator_v<KeyContainer, Allocator>> { };
}

*/

#include <__algorithm/inplace_merge.h>
#include <__algorithm/is_sorted.h>
#include <__algorithm/lexicographical_compare_three_way.h>
#include <__algorithm/partition_point.h>
#include <__algorithm/remove_if.h>
#include <__algorithm/stable_sort.h>
#include <__compare/synth_three_way.h>
#include <__config>
#include <__functional/is_transparent.h>
#include <__functional/operations.h>
#include <__functional/reference_wrapper.h>
#include <__iterator/iterator_traits.h>
#include <__iterator/reverse_iterator.h>
#include <__memory/allocator.h>
#include <__memory/allocator_traits.h>
#include <__memory/uses_allocator.h>
#include <__memory/uses_allocator_construction.h>
#include <__ranges/concepts.h>
#include <__ranges/container_compatible_range.h>
#include <__ranges/from_range.h>
#include <__type_traits/is_allocator.h>
#include <__type_traits/is_constructible.h>
#include <__type_traits/is_convertible.h>
#include <__type_traits/is_nothrow_assignable.h>
#include <__type_traits/is_nothrow_constructible.h>
#include <__type_traits/is_nothrow_default_constructible.h>
#include <__type_traits/is_same.h>
#include <__type_traits/is_trivially_relocatable.h>
#include <__type_traits/type_identity.h>
#include <__utility/exception_guard.h>
#include <__utility/forward.h>
#include <__utility/move.h>
#include <__utility/pair.h>
#include <__utility/sorted_equivalent.h>
#include <vector>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

#if _LIBCPP_STD_VER >= 23

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Key, class _Compare = less<_Key>,
          class _KeyContainer = vector<_Key>>
class _LIBCPP_TRIVIALLY_RELOCATABLE_IF(
  __libcpp_is_trivially_relocatable<_KeyContainer>::value &&
  __libcpp_is_trivially_relocatable<_Compare>::value
) flat_multiset {
  static_assert(is_same_v<_Key, typename _KeyContainer::value_type>);
  static_assert(__has_random_access_iterator_category<typename _KeyContainer::iterator>::value);
  static_assert(__has_random_access_iterator_category<typename _KeyContainer::const_iterator>::value);

public:
  using key_type = _Key;
  using value_type = _Key;
  using key_compare = _Compare;
  using value_compare = _Compare;
  using reference = _Key&;
  using const_reference = const _Key&;
  using size_type = typename _KeyContainer::size_type;
  using difference_type = typename _KeyContainer::difference_type;
  using iterator = typename _KeyContainer::const_iterator;
  using const_iterator = typename _KeyContainer::const_iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using container_type = _KeyContainer;

  // [flat.multiset.cons]

  _LIBCPP_HIDE_FROM_ABI
  flat_multiset()
    noexcept(is_nothrow_default_constructible_v<container_type> && is_nothrow_default_constructible_v<key_compare>) // strengthened
    = default;

  _LIBCPP_HIDE_FROM_ABI
  explicit flat_multiset(container_type __cont)
    : __c_(std::move(__cont)) {
    __construct_invariant();
  }

  template <class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(const container_type& __cont, const _Alloc& __a)
    : __c_(std::__make_obj_using_allocator<container_type>(__a, __cont)) {
    __construct_invariant();
  }

  template <class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(container_type&& __cont, const _Alloc& __a)
    : __c_(std::__make_obj_using_allocator<container_type>(__a, std::move(__cont))) {
    __construct_invariant();
  }

  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(container_type __cont, const key_compare& __comp)
    : __compare_(__comp), __c_(std::move(__cont)) {
    __construct_invariant();
  }

  template <class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(const container_type& __cont, const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp), __c_(std::__make_obj_using_allocator<container_type>(__a, __cont)) {
    __construct_invariant();
  }

  template <class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(container_type&& __cont, const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp), __c_(std::__make_obj_using_allocator<container_type>(__a, std::move(__cont))) {
    __construct_invariant();
  }

  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(sorted_equivalent_t, container_type __cont)
    : __c_(std::move(__cont)) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted(begin(), end()),
        "sorted_equivalent expects input that is sorted");
  }

  template <class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(sorted_equivalent_t, const container_type& __cont, const _Alloc& __a)
    : __c_(std::__make_obj_using_allocator<container_type>(__a, __cont)) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted(begin(), end()),
        "sorted_equivalent expects input that is sorted");
  }

  template <class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(sorted_equivalent_t, container_type&& __cont, const _Alloc& __a)
    : __c_(std::__make_obj_using_allocator<container_type>(__a, std::move(__cont))) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted(begin(), end()),
        "sorted_equivalent expects input that is sorted");
  }

  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(sorted_equivalent_t, container_type __cont, const key_compare& __comp)
    : __compare_(__comp), __c_(std::move(__cont)) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted(begin(), end()),
        "sorted_equivalent expects input that is sorted");
  }

  template <class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(sorted_equivalent_t, const container_type& __cont, const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp), __c_(std::__make_obj_using_allocator<container_type>(__a, __cont)) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted(begin(), end()),
        "sorted_equivalent expects input that is sorted");
  }

  template <class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(sorted_equivalent_t, container_type&& __cont, const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp), __c_(std::__make_obj_using_allocator<container_type>(__a, std::move(__cont))) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted(begin(), end()),
        "sorted_equivalent expects input that is sorted");
  }

  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(const flat_multiset& __fs) = default;

  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(flat_multiset&& __fs)
    noexcept(is_nothrow_constructible_v<_Compare, _Compare&> && is_nothrow_move_constructible_v<_KeyContainer>) // strengthened
#ifndef _LIBCPP_HAS_NO_EXCEPTIONS
    try
#endif
    : __compare_(__fs.__compare_), __c_(std::move(__fs.__c_)) {
    __fs.__c_.clear();
  }
#ifndef _LIBCPP_HAS_NO_EXCEPTIONS
  catch (...) {
    __fs.__restore_invariant();
  }
#endif

  _LIBCPP_HIDE_FROM_ABI
  explicit flat_multiset(const key_compare& __comp)
    : __compare_(__comp) {}

  template <class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp), __c_(std::__make_obj_using_allocator<container_type>(__a)) {}

  template <class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  explicit flat_multiset(const _Alloc& __a)
    : __c_(std::__make_obj_using_allocator<container_type>(__a)) {}

  template <class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(const flat_multiset& __fs, const _Alloc& __a)
    : __compare_(__fs.__compare_), __c_(std::__make_obj_using_allocator<container_type>(__a, __fs.__c_)) {}

  template <class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(flat_multiset&& __fs, const _Alloc& __a)
#ifndef _LIBCPP_HAS_NO_EXCEPTIONS
    try
#endif
    : __compare_(__fs.__compare_), __c_(std::__make_obj_using_allocator<container_type>(__a, std::move(__fs.__c_))) {
    __fs.__c_.clear();
  }
#ifndef _LIBCPP_HAS_NO_EXCEPTIONS
  catch (...) {
    __fs.__restore_invariant();
  }
#endif

  template <class _InputIterator>
    requires __has_input_iterator_category<_InputIterator>::value
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(_InputIterator __first, _InputIterator __last, const key_compare& __comp = key_compare())
    : __compare_(__comp) {
    insert(__first, __last);
  }

  template <class _InputIterator, class _Alloc>
    requires __has_input_iterator_category<_InputIterator>::value &&
             uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(_InputIterator __first, _InputIterator __last, const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp), __c_(std::__make_obj_using_allocator<container_type>(__a)) {
    insert(__first, __last);
  }

  template <class _InputIterator, class _Alloc>
    requires __has_input_iterator_category<_InputIterator>::value &&
             uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(_InputIterator __first, _InputIterator __last, const _Alloc& __a)
    : __c_(std::__make_obj_using_allocator<container_type>(__a)) {
    insert(__first, __last);
  }

  template <_ContainerCompatibleRange<value_type> _Rp>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(from_range_t, _Rp&& __r, const key_compare& __comp = key_compare())
    : flat_multiset(__comp) {
    insert_range(std::forward<_Rp>(__r));
  }

  template <_ContainerCompatibleRange<value_type> _Rp, class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(from_range_t __fr, _Rp&& __r, const _Alloc& __a)
    : flat_multiset(__fr, std::forward<_Rp>(__r), key_compare(), __a) {}

  template <_ContainerCompatibleRange<value_type> _Rp, class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(from_range_t, _Rp&& __r, const key_compare& __comp, const _Alloc& __a)
    : flat_multiset(__comp, __a) {
    insert_range(std::forward<_Rp>(__r));
  }

  template <class _InputIterator>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(sorted_equivalent_t, _InputIterator __first, _InputIterator __last, const key_compare& __comp = key_compare())
    : __compare_(__comp), __c_(__first, __last) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted(begin(), end()),
        "sorted_equivalent expects input that is sorted");
  }

  template <class _InputIterator, class _Alloc>
    requires __has_input_iterator_category<_InputIterator>::value &&
             uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(sorted_equivalent_t, _InputIterator __first, _InputIterator __last, const key_compare& __comp, const _Alloc& __a)
    : __compare_(__comp), __c_(std::__make_obj_using_allocator<container_type>(__a, __first, __last)) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted(begin(), end()),
        "sorted_equivalent expects input that is sorted");
  }

  template <class _InputIterator, class _Alloc>
    requires __has_input_iterator_category<_InputIterator>::value &&
             uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(sorted_equivalent_t, _InputIterator __first, _InputIterator __last, const _Alloc& __a)
    : __c_(std::__make_obj_using_allocator<container_type>(__a, __first, __last)) {
    _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted(begin(), end()),
        "sorted_equivalent expects input that is sorted");
  }

  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(initializer_list<value_type> __il, const key_compare& __comp = key_compare())
    : flat_multiset(__il.begin(), __il.end(), __comp) {}

  template <class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(initializer_list<value_type> __il, const type_identity_t<key_compare>& __comp, const _Alloc& __a)
    : flat_multiset(__il.begin(), __il.end(), __comp, __a) {}

  template <class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(initializer_list<value_type> __il, const _Alloc& __a)
    : flat_multiset(__il.begin(), __il.end(), __a) {}

  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(sorted_equivalent_t __s, initializer_list<value_type> __il,
         const key_compare& __comp = key_compare())
    : flat_multiset(__s, __il.begin(), __il.end(), __comp) {}

  template <class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(sorted_equivalent_t __s, initializer_list<value_type> __il,
           const type_identity_t<key_compare>& __comp, const _Alloc& __a)
    : flat_multiset(__s, __il.begin(), __il.end(), __comp, __a) {}

  template <class _Alloc>
    requires uses_allocator_v<container_type, _Alloc>
  _LIBCPP_HIDE_FROM_ABI
  flat_multiset(sorted_equivalent_t __s, initializer_list<value_type> __il, const _Alloc& __a)
    : flat_multiset(__s, __il.begin(), __il.end(), __a) { }

  _LIBCPP_HIDE_FROM_ABI
  flat_multiset& operator=(initializer_list<value_type> __il) {
    clear();
    insert(__il.begin(), __il.end());
    return *this;
  }

  _LIBCPP_HIDE_FROM_ABI
  flat_multiset& operator=(const flat_multiset& __fs) {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    __compare_ = __fs.__compare_;
    __c_ = __fs.__c_;
    __guard.__complete();
    return *this;
  }

  _LIBCPP_HIDE_FROM_ABI
  flat_multiset& operator=(flat_multiset&& __fs)
    noexcept(is_nothrow_assignable_v<_Compare&, _Compare&> && is_nothrow_assignable_v<_KeyContainer&, _KeyContainer&&>) // strengthened
  {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    __compare_ = __fs.__compare_;
    auto __guard2 = std::__make_exception_guard([&] { __fs.__restore_invariant(); });
    __c_ = std::move(__fs.__c_);
    __guard.__complete();
    // deliberately do not call __guard2.__complete(), so that the clear always happens
    return *this;
  }

  // iterators

  _LIBCPP_HIDE_FROM_ABI iterator begin() noexcept { return __c_.begin(); }
  _LIBCPP_HIDE_FROM_ABI const_iterator begin() const noexcept { return __c_.begin(); }
  _LIBCPP_HIDE_FROM_ABI iterator end() noexcept { return __c_.end(); }
  _LIBCPP_HIDE_FROM_ABI const_iterator end() const noexcept { return __c_.end(); }

  _LIBCPP_HIDE_FROM_ABI reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  _LIBCPP_HIDE_FROM_ABI const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
  _LIBCPP_HIDE_FROM_ABI reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
  _LIBCPP_HIDE_FROM_ABI const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

  _LIBCPP_HIDE_FROM_ABI const_iterator cbegin() const noexcept { return begin(); }
  _LIBCPP_HIDE_FROM_ABI const_iterator cend() const noexcept { return end(); }
  _LIBCPP_HIDE_FROM_ABI const_reverse_iterator crbegin() const noexcept { return rbegin(); }
  _LIBCPP_HIDE_FROM_ABI const_reverse_iterator crend() const noexcept { return rend(); }

  // capacity

  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI bool empty() const noexcept { return __c_.empty(); }
  _LIBCPP_HIDE_FROM_ABI size_type size() const noexcept { return __c_.size(); }
  _LIBCPP_HIDE_FROM_ABI size_type max_size() const noexcept { return __c_.max_size(); }

  // [flat.multiset.modifiers]

  template <class... _Args>
  _LIBCPP_HIDE_FROM_ABI
  iterator emplace(_Args&&... __args) {
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
  iterator insert(const value_type& __x) {
    auto __pos = upper_bound(__x);
    return __insert_unchecked(__pos, __x);
  }

  _LIBCPP_HIDE_FROM_ABI
  iterator insert(value_type&& __x) {
    auto __pos = upper_bound(__x);
    return __insert_unchecked(__pos, std::move(__x));
  }

  template <class _K2>
    requires __is_transparent<_Compare, _K2>::value &&
             is_constructible_v<value_type, _K2>
  _LIBCPP_HIDE_FROM_ABI
  iterator insert(_K2&& __x) {
    auto __pos = upper_bound(__x);
    return __emplace_unchecked(__pos, std::forward<_K2>(__x));
  }

  _LIBCPP_HIDE_FROM_ABI
  iterator insert(const_iterator __pos, const value_type& __x) {
    if (__pos != end() && __compare_(*__pos, __x)) {
      __pos = lower_bound(__x);
    } else if (__pos != begin() && __compare_(__x, __pos[-1])) {
      __pos = upper_bound(__x);
    }
    return __insert_unchecked(__pos, __x);
  }

  _LIBCPP_HIDE_FROM_ABI
  iterator insert(const_iterator __pos, value_type&& __x) {
    if (__pos != end() && __compare_(*__pos, __x)) {
      __pos = lower_bound(__x);
    } else if (__pos != begin() && __compare_(__x, __pos[-1])) {
      __pos = upper_bound(__x);
    }
    return __insert_unchecked(__pos, std::move(__x));
  }

  template <class _K2>
    requires __is_transparent<_Compare, _K2>::value &&
             is_constructible_v<value_type, _K2> &&
             (!is_convertible_v<_K2&&, const_iterator>)
  _LIBCPP_HIDE_FROM_ABI
  iterator insert(const_iterator __pos, _K2&& __x) {
    if (__pos != end() && __compare_(*__pos, __x)) {
      __pos = lower_bound(__x);
    } else if (__pos != begin() && __compare_(__x, __pos[-1])) {
      __pos = upper_bound(__x);
    }
    return __emplace_unchecked(__pos, std::forward<_K2>(__x));
  }

  template <class _InputIterator>
    requires __has_input_iterator_category<_InputIterator>::value
  _LIBCPP_HIDE_FROM_ABI
  void insert(_InputIterator __first, _InputIterator __last) {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    auto __it = __c_.insert(__c_.end(), __first, __last);
    auto __end = __c_.end();
    if (__it != __end) {
      std::stable_sort(__it, __end, __comp_ref());
      std::inplace_merge(__c_.begin(), __it, __end, __comp_ref());
    }
    __guard.__complete();
  }

  template <class _InputIterator>
    requires __has_input_iterator_category<_InputIterator>::value
  _LIBCPP_HIDE_FROM_ABI
  void insert(sorted_equivalent_t, _InputIterator __first, _InputIterator __last) {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    auto __it = __c_.insert(__c_.end(), __first, __last);
    auto __end = __c_.end();
    if (__it != __end) {
      _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted(__it, __end),
          "sorted_equivalent expects input that is sorted");
      std::inplace_merge(__c_.begin(), __it, __end, __comp_ref());
    }
    __guard.__complete();
  }

  template <_ContainerCompatibleRange<value_type> _Rp>
  _LIBCPP_HIDE_FROM_ABI
  void insert_range(_Rp&& __r) {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    size_type __n = 0;
    for (auto&& __e : __r) {
      __c_.insert(__c_.end(), decltype(__e)(__e));
      ++__n;
    }
    if (__n != 0) {
      auto __end = __c_.end();
      auto __it = __end - __n;
      std::stable_sort(__it, __end, __comp_ref());
      std::inplace_merge(__c_.begin(), __it, __end, __comp_ref());
    }
    __guard.__complete();
  }

  _LIBCPP_HIDE_FROM_ABI
  void insert(initializer_list<value_type> __il) { insert(__il.begin(), __il.end()); }

  _LIBCPP_HIDE_FROM_ABI
  void insert(sorted_equivalent_t __s, initializer_list<value_type> __il) { insert(__s, __il.begin(), __il.end()); }

  _LIBCPP_HIDE_FROM_ABI
  container_type extract() && {
    auto __guard = std::__make_exception_guard([&] { clear(); });
    container_type __res = std::move(__c_);
    // deliberately do not call __guard.__complete(), so that the clear always happens
    return __res;
  }

  _LIBCPP_HIDE_FROM_ABI
  void replace(container_type&& __cont) {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    __c_ = std::move(__cont);
    __guard.__complete();
    _LIBCPP_ASSERT_UNCATEGORIZED(__is_sorted(begin(), end()),
        "replace expects input that is sorted");
  }

  _LIBCPP_HIDE_FROM_ABI
  iterator erase(const_iterator __pos) {
    // Notice that our iterator and const_iterator are the same type
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    auto __it = __c_.erase(__pos);
    __guard.__complete();
    return __it;
  }

  _LIBCPP_HIDE_FROM_ABI
  size_type erase(const key_type& __x) {
    auto [__first, __last] = equal_range(__x);
    auto __n = __last - __first;
    if (__n) {
      erase(__first, __last);
    }
    return __n;
  }

  template <class _K2>
    requires __is_transparent<_Compare, _K2>::value &&
             (!is_convertible_v<_K2&&, iterator>)
  _LIBCPP_HIDE_FROM_ABI
  size_type erase(_K2&& __x) {
    // Notice that our iterator and const_iterator are the same type
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
    auto __it = __c_.erase(__first, __last);
    __guard.__complete();
    return __it;
  }

  _LIBCPP_HIDE_FROM_ABI
  value_type displace(const_iterator __pos) {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    auto __it = __c_.erase(__pos, __pos);
    auto __v = std::move(*__it);
    __c_.erase(__it);
    __guard.__complete();
    return __v;
  }

  _LIBCPP_HIDE_FROM_ABI
  void swap(flat_multiset& __y) noexcept {
    using std::swap;
    swap(__compare_, __y.__compare_);
    swap(__c_, __y.__c_);
  }

  _LIBCPP_HIDE_FROM_ABI
  void clear() noexcept {
    __c_.clear();
  }

  // observers

  _LIBCPP_HIDE_FROM_ABI key_compare key_comp() const { return __compare_; }
  _LIBCPP_HIDE_FROM_ABI value_compare value_comp() const { return __compare_; }

  // set operations

  _LIBCPP_HIDE_FROM_ABI
  const_iterator find(const key_type& __x) const {
    // Notice that our iterator and const_iterator are the same type
    auto __it = lower_bound(__x);
    if (__it == end() || __compare_(__x, *__it)) {
      return end();
    }
    return __it;
  }

  template <class _K2>
    requires __is_transparent<_Compare, _K2>::value
  _LIBCPP_HIDE_FROM_ABI
  const_iterator find(const _K2& __x) const {
    auto __it = lower_bound(__x);
    if (__it == end() || __compare_(__x, *__it)) {
      return end();
    }
    return __it;
  }

  _LIBCPP_HIDE_FROM_ABI
  size_type count(const key_type& __x) const {
    auto [__first, __last] = equal_range(__x);
    return __last - __first;
  }

  template <class _K2>
    requires __is_transparent<_Compare, _K2>::value
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
    requires __is_transparent<_Compare, _K2>::value
  _LIBCPP_HIDE_FROM_ABI
  bool contains(const _K2& __x) const {
    return find(__x) != end();
  }

  _LIBCPP_HIDE_FROM_ABI
  const_iterator lower_bound(const key_type& __x) const {
    // Notice that our iterator and const_iterator are the same type
    return std::partition_point(begin(), end(), [&](const value_type& __e) {
      return __compare_(__e, __x);
    });
  }

  template <class _K2>
    requires __is_transparent<_Compare, _K2>::value
  _LIBCPP_HIDE_FROM_ABI
  const_iterator lower_bound(const _K2& __x) const {
    // Notice that our iterator and const_iterator are the same type
    return std::partition_point(begin(), end(), [&](const value_type& __e) {
      return __compare_(__e, __x);
    });
  }

  _LIBCPP_HIDE_FROM_ABI
  const_iterator upper_bound(const key_type& __x) const {
    // Notice that our iterator and const_iterator are the same type
    return std::partition_point(begin(), end(), [&](const value_type& __e) {
      return !__compare_(__x, __e);
    });
  }

  template <class _K2>
    requires __is_transparent<_Compare, _K2>::value
  _LIBCPP_HIDE_FROM_ABI
  const_iterator upper_bound(const _K2& __x) const {
    // Notice that our iterator and const_iterator are the same type
    return std::partition_point(begin(), end(), [&](const value_type& __e) {
      return !__compare_(__x, __e);
    });
  }

  _LIBCPP_HIDE_FROM_ABI
  pair<const_iterator, const_iterator> equal_range(const key_type& __x) const {
    // Notice that our iterator and const_iterator are the same type
    auto __end = end();
    auto __first = std::partition_point(begin(), __end, [&](const value_type& __e) {
      return __compare_(__e, __x);
    });
    auto __last = std::partition_point(__first, __end, [&](const value_type& __e) {
      return !__compare_(__x, __e);
    });
    return {__first, __last};
  }

  template <class _K2>
    requires __is_transparent<_Compare, _K2>::value
  _LIBCPP_HIDE_FROM_ABI
  pair<const_iterator, const_iterator> equal_range(const _K2& __x) const {
    // Notice that our iterator and const_iterator are the same type
    auto __end = end();
    auto __first = std::partition_point(begin(), __end, [&](const value_type& __e) {
      return __compare_(__e, __x);
    });
    auto __last = std::partition_point(__first, __end, [&](const value_type& __e) {
      return !__compare_(__x, __e);
    });
    return {__first, __last};
  }

  _LIBCPP_HIDE_FROM_ABI
  friend bool operator==(const flat_multiset& __x, const flat_multiset& __y) {
    return __x.__c_ == __y.__c_;
  }

  _LIBCPP_HIDE_FROM_ABI
  friend auto operator<=>(const flat_multiset& __x, const flat_multiset& __y)
    requires requires (const_iterator __it) { __synth_three_way(*__it, *__it); }
  {
    return std::lexicographical_compare_three_way(__x.begin(), __x.end(), __y.begin(), __y.end(), __synth_three_way<value_type, value_type>);
  }

  _LIBCPP_HIDE_FROM_ABI
  friend void swap(flat_multiset& __x, flat_multiset& __y) noexcept {
    __x.swap(__y);
  }

  template <class _Predicate>
  _LIBCPP_HIDE_FROM_ABI
  size_type __erase_if(reference_wrapper<const _Predicate> __pred) {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    auto __end = __c_.end();
    auto __it = std::remove_if(__c_.begin(), __end, __pred);
    auto __n = __end - __it;
    __c_.erase(__it, __end);
    __guard.__complete();
    return __n;
  }

private:
  _LIBCPP_HIDE_FROM_ABI
  void __construct_invariant() {
     // This function must not be called except from the constructors,
     // where if the comparator or move-assignment throws an exception
     // we will have no flat_multiset object and thus no invariant to restore.
     auto __begin = __c_.begin();
     auto __end = __c_.end();
     std::stable_sort(__begin, __end, __comp_ref());
  }

  _LIBCPP_HIDE_FROM_ABI
  void __restore_invariant() {
    // There is already an exception in flight, so we cannot do anything
    // that would risk a second exception. In particular, the comparator
    // might throw, or value_type's operator= might throw.
    // All we can safely do, in general, is clear the container.
    __c_.clear();
  }

  _LIBCPP_HIDE_FROM_ABI
  bool __is_sorted(const_iterator __first, const_iterator __last) const {
    return std::is_sorted(__first, __last, __comp_ref());
  }

  _LIBCPP_HIDE_FROM_ABI
  reference_wrapper<const _Compare> __comp_ref() const {
    return reference_wrapper<const _Compare>(__compare_);
  }

  template <class _K2>
  _LIBCPP_HIDE_FROM_ABI
  iterator __insert_unchecked(const_iterator __pos, _K2&& __x) {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    __pos = __c_.insert(__pos, std::forward<_K2>(__x));
    __guard.__complete();
    return __pos;
  }

  template <class _K2>
  _LIBCPP_HIDE_FROM_ABI
  iterator __emplace_unchecked(const_iterator __pos, _K2&& __x) {
    auto __guard = std::__make_exception_guard([&] { __restore_invariant(); });
    __pos = __c_.emplace(__pos, std::forward<_K2>(__x));
    __guard.__complete();
    return __pos;
  }

  _LIBCPP_NO_UNIQUE_ADDRESS key_compare __compare_ = key_compare();
  container_type __c_ = container_type();
};

template<class _KeyContainer,
         class _Compare = less<typename _KeyContainer::value_type>,
         class = typename _KeyContainer::const_iterator,
         class = enable_if_t<!__is_allocator<_Compare>::value>>
flat_multiset(_KeyContainer, _Compare = _Compare())
  -> flat_multiset<typename _KeyContainer::value_type, _Compare, _KeyContainer>;

template<class _KeyContainer,
         class _Compare,
         class _Alloc,
         class = typename _KeyContainer::const_iterator,
         class = enable_if_t<!__is_allocator<_Compare>::value>,
         class = enable_if_t<uses_allocator_v<_KeyContainer, _Alloc>>>
flat_multiset(_KeyContainer, _Compare, _Alloc)
  -> flat_multiset<typename _KeyContainer::value_type, _Compare, _KeyContainer>;

template<class _KeyContainer,
         class _Alloc,
         class = typename _KeyContainer::const_iterator,
         class = enable_if_t<uses_allocator_v<_KeyContainer, _Alloc>>>
  requires true
flat_multiset(_KeyContainer, _Alloc)
  -> flat_multiset<typename _KeyContainer::value_type, less<typename _KeyContainer::value_type>, _KeyContainer>;

template<class _KeyContainer,
         class _Compare = less<typename _KeyContainer::value_type>,
         class = typename _KeyContainer::const_iterator,
         class = enable_if_t<!__is_allocator<_Compare>::value>>
flat_multiset(sorted_equivalent_t, _KeyContainer, _Compare = _Compare())
  -> flat_multiset<typename _KeyContainer::value_type, _Compare, _KeyContainer>;

template<class _KeyContainer,
         class _Compare,
         class _Alloc,
         class = typename _KeyContainer::const_iterator,
         class = enable_if_t<!__is_allocator<_Compare>::value>,
         class = enable_if_t<uses_allocator_v<_KeyContainer, _Alloc>>>
flat_multiset(sorted_equivalent_t, _KeyContainer, _Compare, _Alloc)
  -> flat_multiset<typename _KeyContainer::value_type, _Compare, _KeyContainer>;

template<class _KeyContainer,
         class _Alloc,
         class = typename _KeyContainer::const_iterator,
         class = enable_if_t<uses_allocator_v<_KeyContainer, _Alloc>>>
  requires true
flat_multiset(sorted_equivalent_t, _KeyContainer, _Alloc)
  -> flat_multiset<typename _KeyContainer::value_type, less<typename _KeyContainer::value_type>, _KeyContainer>;

template<class _InputIterator,
         class _Compare = less<__iter_value_type<_InputIterator>>,
         class _Allocator = allocator<int>,
         class = enable_if_t<__has_input_iterator_category<_InputIterator>::value>,
         class = enable_if_t<!__is_allocator<_Compare>::value>,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_multiset(_InputIterator, _InputIterator, _Compare = _Compare(), _Allocator = _Allocator())
  -> flat_multiset<__iter_value_type<_InputIterator>, _Compare,
              vector<__iter_value_type<_InputIterator>, __allocator_traits_rebind_t<_Allocator, __iter_value_type<_InputIterator>>>>;

template<class _InputIterator,
         class _Compare = less<__iter_value_type<_InputIterator>>,
         class _Allocator = allocator<int>,
         class = enable_if_t<__has_input_iterator_category<_InputIterator>::value>,
         class = enable_if_t<!__is_allocator<_Compare>::value>,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_multiset(sorted_equivalent_t, _InputIterator, _InputIterator, _Compare = _Compare(), _Allocator = _Allocator())
  -> flat_multiset<__iter_value_type<_InputIterator>, _Compare,
              vector<__iter_value_type<_InputIterator>, __allocator_traits_rebind_t<_Allocator, __iter_value_type<_InputIterator>>>>;

template<class _InputIterator,
         class _Allocator,
         class = enable_if_t<__has_input_iterator_category<_InputIterator>::value>,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_multiset(_InputIterator, _InputIterator, _Allocator)
  -> flat_multiset<__iter_value_type<_InputIterator>, less<__iter_value_type<_InputIterator>>,
              vector<__iter_value_type<_InputIterator>, __allocator_traits_rebind_t<_Allocator, __iter_value_type<_InputIterator>>>>;

template<class _InputIterator,
         class _Allocator = allocator<int>,
         class = enable_if_t<__has_input_iterator_category<_InputIterator>::value>,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_multiset(sorted_equivalent_t, _InputIterator, _InputIterator, _Allocator)
  -> flat_multiset<__iter_value_type<_InputIterator>, less<__iter_value_type<_InputIterator>>,
              vector<__iter_value_type<_InputIterator>, __allocator_traits_rebind_t<_Allocator, __iter_value_type<_InputIterator>>>>;

template<ranges::input_range _Rp,
         class _Compare = less<ranges::range_value_t<_Rp>>,
         class _Allocator = allocator<int>,
         class = enable_if_t<!__is_allocator<_Compare>::value>,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_multiset(from_range_t, _Rp&&, _Compare = _Compare(), _Allocator = _Allocator())
  -> flat_multiset<ranges::range_value_t<_Rp>, _Compare,
              vector<ranges::range_value_t<_Rp>, __allocator_traits_rebind_t<_Allocator, ranges::range_value_t<_Rp>>>>;

template<ranges::input_range _Rp,
         class _Allocator,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_multiset(from_range_t, _Rp&&, _Allocator)
  -> flat_multiset<ranges::range_value_t<_Rp>, less<ranges::range_value_t<_Rp>>,
              vector<ranges::range_value_t<_Rp>, __allocator_traits_rebind_t<_Allocator, ranges::range_value_t<_Rp>>>>;

template<class _Key,
         class _Compare = less<_Key>,
         class _Allocator = allocator<_Key>,
         class = enable_if_t<!__is_allocator<_Compare>::value>,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_multiset(initializer_list<_Key>, _Compare = _Compare(), _Allocator = _Allocator())
  -> flat_multiset<_Key, _Compare,
              vector<_Key, __allocator_traits_rebind_t<_Allocator, _Key>>>;

template<class _Key,
         class _Compare = less<_Key>,
         class _Allocator = allocator<_Key>,
         class = enable_if_t<!__is_allocator<_Compare>::value>,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_multiset(sorted_equivalent_t, initializer_list<_Key>, _Compare = _Compare(), _Allocator = _Allocator())
  -> flat_multiset<_Key, _Compare,
              vector<_Key, __allocator_traits_rebind_t<_Allocator, _Key>>>;

template<class _Key,
         class _Allocator,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_multiset(initializer_list<_Key>, _Allocator)
  -> flat_multiset<_Key, less<_Key>,
              vector<_Key, __allocator_traits_rebind_t<_Allocator, _Key>>>;

template<class _Key,
         class _Allocator,
         class = enable_if_t<__is_allocator<_Allocator>::value>>
flat_multiset(sorted_equivalent_t, initializer_list<_Key>, _Allocator)
  -> flat_multiset<_Key, less<_Key>,
              vector<_Key, __allocator_traits_rebind_t<_Allocator, _Key>>>;

template <class _Key, class _Compare, class _KeyContainer, class _Alloc>
struct uses_allocator<flat_multiset<_Key, _Compare, _KeyContainer>, _Alloc>
  : bool_constant<uses_allocator_v<_KeyContainer, _Alloc>> { };

// [flat.set.erasure]

template <class _Key, class _Compare, class _KeyContainer, class _Predicate>
_LIBCPP_HIDE_FROM_ABI
auto erase_if(flat_multiset<_Key, _Compare, _KeyContainer>& __c, _Predicate __pred)
    -> typename flat_multiset<_Key, _Compare, _KeyContainer>::size_type
{
    return __c.__erase_if(reference_wrapper<const _Predicate>(__pred));
}

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP_STD_VER >= 23

#endif // _LIBCPP___FLAT_SET_FLAT_MULTISET_H
