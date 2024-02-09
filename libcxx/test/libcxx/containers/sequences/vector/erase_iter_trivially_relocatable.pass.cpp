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

#include <vector>
#include <cassert>

static int constructions = 0;
static int assignments = 0;
static int destructions = 0;

template<bool TR>
struct _LIBCPP_TRIVIALLY_RELOCATABLE_IF(TR) MyClass {
    int value;
    explicit MyClass(int i) : value(i) {
        ++constructions;
    }
    MyClass(const MyClass& rhs) {
        ++constructions;
        value = rhs.value;
    }
    MyClass(MyClass&& rhs) noexcept {
        ++constructions;
        value = rhs.value;
    }
    MyClass& operator=(const MyClass& rhs) {
        ++assignments;
        value = rhs.value;
        return *this;
    }
    MyClass& operator=(MyClass&& rhs) {
        ++assignments;
        value = rhs.value;
        return *this;
    }
    ~MyClass() {
        ++destructions;
    }
};

template<bool TR>
void test()
{
    using T = MyClass<TR>;
    std::vector<T> vec;
    vec.push_back(T(1));
    vec.push_back(T(2));
    vec.push_back(T(3));
    vec.push_back(T(4));
    vec.push_back(T(5));

    constructions = 0;
    assignments = 0;
    destructions = 0;
    vec.erase(vec.begin() + 1);
    if (TR) {
        assert(constructions == 0);
        assert(assignments == 0);  // relocate without assigning
        assert(destructions == 1);
    } else {
        assert(constructions == 0);
        assert(assignments == 3);
        assert(destructions == 1);
    }
    assert(vec.size() == 4);
    assert(vec[0].value == 1);
    assert(vec[1].value == 3);
    assert(vec[2].value == 4);
    assert(vec[3].value == 5);

    constructions = 0;
    assignments = 0;
    destructions = 0;
    vec.erase(vec.end() - 1);
    if (true) {
        assert(constructions == 0);
        assert(assignments == 0);
        assert(destructions == 1);
    }
    assert(vec.size() == 3);
    assert(vec[0].value == 1);
    assert(vec[1].value == 3);
    assert(vec[2].value == 4);
}

int main(int, char**)
{
    test<true>();
    test<false>();
    return 0;
}
