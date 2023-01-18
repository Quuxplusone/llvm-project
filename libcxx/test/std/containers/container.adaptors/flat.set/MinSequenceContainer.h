//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef SUPPORT_MIN_SEQUENCE_CONTAINER_H
#define SUPPORT_MIN_SEQUENCE_CONTAINER_H

#include <initializer_list>
#include <vector>

#include "test_iterators.h"

template<class T>
struct MinSequenceContainer {
  using value_type = T;
  using difference_type = short;
  using size_type = unsigned short;
  using iterator = random_access_iterator<T*>;
  using const_iterator = random_access_iterator<const T*>;

  explicit MinSequenceContainer() = default;
  template<class It> explicit MinSequenceContainer(It first, It last) : data_(first, last) {}
  MinSequenceContainer(std::initializer_list<T> il) : data_(il) {}
  iterator begin() { return iterator(data_.data()); }
  const_iterator begin() const { return const_iterator(data_.data()); }
  const_iterator cbegin() const { return const_iterator(data_.data()); }
  iterator end() { return begin() + size(); }
  const_iterator end() const { return begin() + size(); }
  size_type size() const { return data_.size(); }
  bool empty() const { return data_.empty(); }

  void clear() { data_.clear(); }
  template<class It>
  iterator insert(const_iterator p, It first, It last) {
    auto it = data_.insert(
      p - cbegin() + data_.begin(),
      first, last
    );
    return it - data_.begin() + begin();
  }
  iterator insert(const_iterator p, int value) {
    auto it = data_.insert(
      p - cbegin() + data_.begin(),
      value
    );
    return it - data_.begin() + begin();
  }
  iterator erase(const_iterator first, const_iterator last) {
    auto it = data_.erase(first - cbegin() + data_.begin(), last - cbegin() + data_.begin());
    return it - data_.begin() + begin();
  }

private:
  std::vector<T> data_;
};

#endif // SUPPORT_MIN_SEQUENCE_CONTAINER_H
