//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <vector>

// iterator emplace(const_iterator pos, Args&&... args);
// iterator insert(const_iterator pos, const value_type& value);
// iterator insert(const_iterator pos, value_type&& value);
// iterator insert(const_iterator pos, InputIt first, InputIt last);
// iterator erase(const_iterator pos);
// iterator erase(const_iterator first, const_iterator last);

#include <cassert>
#include <list>
#include <map>
#include <new>
#include <string>
#include <typeinfo>
#include <vector>

static std::map<void*, const std::type_info*> constructed_addresses;

template<class T>
struct Alloc {
    using value_type = T;

    static T *allocate(size_t n) { return (T*)::operator new(n * sizeof(T)); }
    static void deallocate(T *p, size_t) { return ::operator delete(p); }

    template<class U, class... Args>
    static void construct(U *p, Args&&... args) {
        auto kv = constructed_addresses.emplace(p, &typeid(U));
        assert(kv.second); // or else we've double-constructed something at this address
        ::new (p) U(std::forward<Args>(args)...);
    }

    template<class U>
    static void destroy(U *p) {
        auto it = constructed_addresses.find(p);
        assert(it != constructed_addresses.end()); // or else we're destroying an object we didn't construct
        assert(it->second == &typeid(U)); // or else we're destroying an object of the wrong type
        size_t n = constructed_addresses.erase(p);
        assert(n == 1); // or else we're destroying an object we didn't construct
        p->~U();
    }

    explicit Alloc() = default;
    Alloc(const Alloc&) = default;
    template<class U> Alloc(const Alloc<U>&) { }
    Alloc& operator=(const Alloc&) = default;
    ~Alloc() = default;
};

template<class F>
void test(F f)
{
  using T = decltype(f(1));
  std::vector<T, Alloc<T>> vec;
  {
    vec.push_back(f(1));
    vec.push_back(f(2));
    vec.push_back(f(3));
    vec.push_back(f(4));
    vec.push_back(f(5));
    assert(constructed_addresses.size() == 5);
  }
  {
    vec.erase(vec.begin() + 1, vec.begin() + 3);
    assert(vec.size() == 3);
    assert(constructed_addresses.size() == 3);

    vec.erase(vec.begin(), vec.begin() + 1);
    assert(vec.size() == 2);
    assert(constructed_addresses.size() == 2);
  }
  {
    vec = {f(1), f(2), f(3), f(4), f(5)};
    assert(constructed_addresses.size() == 5);
    vec.reserve(15);
    assert(constructed_addresses.size() == 5);
  }
  {
    vec.erase(vec.begin() + 1);
    assert(vec.size() == 4);
    assert(constructed_addresses.size() == 4);

    vec.erase(vec.end() - 1);
    assert(vec.size() == 3);
    assert(constructed_addresses.size() == 3);
  }
  {
    vec.insert(vec.begin(), f(6));
    assert(vec.size() == 4);
    assert(constructed_addresses.size() == 4);

    T lvalue = f(7);
    vec.insert(vec.end() - 2, lvalue);
    assert(vec.size() == 5);
    assert(constructed_addresses.size() == 5);
  }
  {
    T a[2] = { f(8), f(9) };
    vec.insert(vec.begin(), a, a + 2);
    assert(vec.size() == 7);
    assert(constructed_addresses.size() == 7);

    vec.insert(vec.end(), a, a + 2);
    assert(vec.size() == 9);
    assert(constructed_addresses.size() == 9);

    vec.insert(vec.end() - 1, a, a + 2);
    assert(vec.size() == 11);
    assert(constructed_addresses.size() == 11);

    vec.insert(vec.end() - 2, a, a + 2);
    assert(vec.size() == 13);
    assert(constructed_addresses.size() == 13);

    vec.insert(vec.end() - 3, a, a + 2);
    assert(vec.size() == 15);
    assert(constructed_addresses.size() == 15);
  }
  vec.clear();
  assert(constructed_addresses.size() == 0);
}

int main(int, char**) {
  {
    test([](int i) { return i; }); // trivially copyable
    test([](int i) { return std::to_string(i); }); // trivially relocatable
    test([](int i) { return std::list<int>(i, 42); }); // not trivially relocatable
  }
  return 0;
}
