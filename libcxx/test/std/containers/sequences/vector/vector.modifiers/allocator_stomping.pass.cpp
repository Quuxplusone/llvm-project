//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <vector>

// iterator erase(const_iterator pos);
// iterator erase(const_iterator first, const_iterator last);

#include <cassert>
#include <cstring>
#include <memory>
#include <vector>

template<class T>
struct Alloc : std::allocator<T> {
    template<class U>
    struct rebind {
        using other = Alloc<U>;
    };
    template<class U>
    void destroy(U *p) {
        p->~U();
        std::memset(p, 0xDE, sizeof(U));
    }
};

int main(int, char**) {
  {
    std::vector<unsigned, Alloc<unsigned>> v = {1,2,3,4,5};
    v.erase(v.begin());
    assert((v == decltype(v){2,3,4,5}));
    const unsigned *p = v.data();
    // The following line is probably UB, but in practice it'll be okay
    // because we haven't deallocated that memory yet. Check that the
    // allocator's `destroy` method ran and stomped on the bytes.
    assert(p[4] == 0xDEDEDEDE);
  }
  {
    std::vector<unsigned, Alloc<unsigned>> v = {1,2,3,4,5};
    v.erase(v.begin(), v.begin() + 2);
    assert((v == decltype(v){3,4,5}));
    const unsigned *p = v.data();
    // The following line is probably UB, but in practice it'll be okay
    // because we haven't deallocated that memory yet. Check that the
    // allocator's `destroy` method ran and stomped on the bytes.
    assert(p[3] == 0xDEDEDEDE);
    assert(p[4] == 0xDEDEDEDE);
  }
  {
    std::vector<unsigned, Alloc<unsigned>> v = {1,2,3,4,5};
    v.erase(v.begin(), v.begin() + 3);
    assert((v == decltype(v){4,5}));
    const unsigned *p = v.data();
    // The following line is probably UB, but in practice it'll be okay
    // because we haven't deallocated that memory yet. Check that the
    // allocator's `destroy` method ran and stomped on the bytes.
    assert(p[2] == 0xDEDEDEDE);
    assert(p[3] == 0xDEDEDEDE);
    assert(p[4] == 0xDEDEDEDE);
  }
  return 0;
}

