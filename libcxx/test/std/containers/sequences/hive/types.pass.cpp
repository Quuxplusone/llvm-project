//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

//  template<class T, class Allocator>
//  class hive {
//  public:
//    using value_type = T;
//    using allocator_type = Allocator;
//    using pointer = typename allocator_traits<Allocator>::pointer;
//    using const_pointer = typename allocator_traits<Allocator>::const_pointer;
//    using reference = value_type&;
//    using const_reference = const value_type&;
//    using size_type = implementation-defined; // see [container.requirements]
//    using difference_type = implementation-defined; // see [container.requirements]
//    using iterator = implementation-defined; // see [container.requirements]
//    using const_iterator = implementation-defined; // see [container.requirements]
//    using reverse_iterator = std::reverse_iterator<iterator>; // see [container.requirements]
//    using const_reverse_iterator = std::reverse_iterator<const_iterator>; // see [container.requirements]

#include <hive>
#include <type_traits>

#include "test_macros.h"
#include "min_allocator.h"

int main(int, char**)
{
  {
    using C = std::hive<int>;
    static_assert(std::is_same_v<C::value_type, int>);
    static_assert(std::is_same_v<C::reference, int&>);
    static_assert(std::is_same_v<C::const_reference, const int&>);
    static_assert(std::is_same_v<C::pointer, int*>);
    static_assert(std::is_same_v<C::const_pointer, const int*>);
    static_assert(std::is_same_v<C::size_type, std::size_t>);
    static_assert(std::is_same_v<C::difference_type, std::ptrdiff_t>);
    static_assert(std::is_same_v<C::reverse_iterator, std::reverse_iterator<C::iterator>>);
    static_assert(std::is_same_v<C::const_reverse_iterator, std::reverse_iterator<C::const_iterator>>);
  }
  {
    using C = std::hive<short, min_allocator<short>>;
    static_assert(std::is_same_v<C::value_type, short>);
    static_assert(std::is_same_v<C::reference, short&>);
    static_assert(std::is_same_v<C::const_reference, const short&>);
    static_assert(std::is_same_v<C::pointer, std::allocator_traits<min_allocator<short>>::pointer>);
    static_assert(std::is_same_v<C::const_pointer, std::allocator_traits<min_allocator<short>>::const_pointer>);
    // min_allocator doesn't have a size_type, so one gets synthesized
    static_assert(std::is_same_v<C::size_type, std::make_unsigned_t<C::difference_type>>);
    static_assert(std::is_same_v<C::difference_type, std::ptrdiff_t>);
    static_assert(std::is_same_v<C::reverse_iterator, std::reverse_iterator<C::iterator>>);
    static_assert(std::is_same_v<C::const_reverse_iterator, std::reverse_iterator<C::const_iterator>>);
  }
  return 0;
}
