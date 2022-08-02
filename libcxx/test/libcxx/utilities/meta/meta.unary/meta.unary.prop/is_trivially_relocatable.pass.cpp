//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// __libcpp_is_trivially_relocatable
// __libcpp_is_trivially_relocatable_v

#include <memory>
#include <type_traits>
#include "test_macros.h"


template <class T>
void test_is_trivially_relocatable()
{
    static_assert(std::__libcpp_is_trivially_relocatable<T>::value, "");
#if TEST_STD_VER > 14
    static_assert(std::__libcpp_is_trivially_relocatable_v<T>, "");
#endif
}

template <class T>
void test_is_not_trivially_relocatable()
{
    static_assert(!std::__libcpp_is_trivially_relocatable<T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::__libcpp_is_trivially_relocatable_v<T>, "");
#endif
}

class Empty {};

class Abstract {
public:
  virtual ~Abstract() = 0;
};

class Polymorphic {
public:
  virtual ~Polymorphic() = default;
};

union Union {};

struct bit_zero {
  int : 0;
};

struct CopyCtor {
  CopyCtor(const CopyCtor&);
};

#if TEST_STD_VER >= 11
struct MoveOnly1 {
  MoveOnly1(MoveOnly1&&);
};

struct MoveOnly2 {
  MoveOnly2(MoveOnly2&&) = default;
};
#endif

int main(int, char**)
{
    test_is_not_trivially_relocatable<void>();
    test_is_not_trivially_relocatable<const void>();
    test_is_not_trivially_relocatable<Abstract>();
    test_is_not_trivially_relocatable<Polymorphic>();
    test_is_not_trivially_relocatable<CopyCtor>();

    test_is_trivially_relocatable<Union>();
    test_is_trivially_relocatable<Empty>();
    test_is_trivially_relocatable<double>();
    test_is_trivially_relocatable<int*>();
    test_is_trivially_relocatable<const int*>();
    test_is_trivially_relocatable<bit_zero>();

    test_is_trivially_relocatable<int>();
    test_is_trivially_relocatable<const int>();
    test_is_trivially_relocatable<volatile int>();
    test_is_trivially_relocatable<const volatile int>();

    test_is_trivially_relocatable<int[]>();
    test_is_trivially_relocatable<int[10]>();
    test_is_trivially_relocatable<int[][2]>();
    test_is_trivially_relocatable<int[1][2]>();

#if TEST_STD_VER >= 11
    test_is_not_trivially_relocatable<MoveOnly1>();
    test_is_not_trivially_relocatable<const MoveOnly1>();
    test_is_trivially_relocatable<MoveOnly2>();
    test_is_trivially_relocatable<const MoveOnly2>();
#endif

    return 0;
}
