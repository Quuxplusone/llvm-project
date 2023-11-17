//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-exceptions
// <exception>

// template<class E> const E* try_cast(const exception_ptr& p);
// Mandates:
// - T is a complete object type.
// - T is not cv-qualified.
// - T is not an array type.

#include <exception>

struct Incomplete;
struct Base { int x; int f(); };

int main(int, char**)
{
    std::exception_ptr p;
    (void)std::try_cast<void>(p); //expected-error@*:* {{invalid application of 'sizeof' to an incomplete type 'void'}}
    (void)std::try_cast<int&>(p); //expected-error@*:* {{E must be a complete cv-unqualified object type, and not an array type}}
    (void)std::try_cast<int&&>(p); //expected-error@*:* {{E must be a complete cv-unqualified object type, and not an array type}}
    (void)std::try_cast<int*&>(p); //expected-error@*:* {{E must be a complete cv-unqualified object type, and not an array type}}
    (void)std::try_cast<const int>(p); //expected-error@*:* {{E must be a complete cv-unqualified object type, and not an array type}}
    (void)std::try_cast<volatile int>(p); //expected-error@*:* {{E must be a complete cv-unqualified object type, and not an array type}}
        //expected-error@*:* {{cannot initialize return object}}
    (void)std::try_cast<Incomplete>(p); //expected-error@*:* {{invalid application of 'sizeof' to an incomplete type 'Incomplete'}}
    (void)std::try_cast<int()>(p); //expected-error@*:* {{invalid application of 'sizeof' to a function type}}
    (void)std::try_cast<int[]>(p); //expected-error@*:* {{invalid application of 'sizeof' to an incomplete type 'int[]'}}
    (void)std::try_cast<int[5]>(p); //expected-error@*:* {{E must be a complete cv-unqualified object type, and not an array type}}
    (void)std::try_cast<int(&)()>(p); //expected-error@*:* {{invalid application of 'sizeof' to a function type}}
    (void)std::try_cast<int(&)[]>(p); //expected-error@*:* {{invalid application of 'sizeof' to an incomplete type 'int[]'}}
    (void)std::try_cast<int(&)[5]>(p); //expected-error@*:* {{E must be a complete cv-unqualified object type}}

    (void)std::try_cast<void*>(p); //expected-error@*:* {{E must not be a pointer or pointer-to-member type}}
    (void)std::try_cast<int*>(p); //expected-error@*:* {{E must not be a pointer or pointer-to-member type}}
    (void)std::try_cast<int(*)[5]>(p); //expected-error@*:* {{E must not be a pointer or pointer-to-member type}}
    (void)std::try_cast<Base*>(p); //expected-error@*:* {{E must not be a pointer or pointer-to-member type}}
    (void)std::try_cast<const Base*>(p); //expected-error@*:* {{E must not be a pointer or pointer-to-member type}}
    (void)std::try_cast<Incomplete*>(p); //expected-error@*:* {{E must not be a pointer or pointer-to-member type}}
    (void)std::try_cast<int(*)()>(p); //expected-error@*:* {{E must not be a pointer or pointer-to-member type}}
    (void)std::try_cast<int Base::*>(p); //expected-error@*:* {{E must not be a pointer or pointer-to-member type}}
    (void)std::try_cast<int (Base::*)()>(p); //expected-error@*:* {{E must not be a pointer or pointer-to-member type}}

    return 0;
}
