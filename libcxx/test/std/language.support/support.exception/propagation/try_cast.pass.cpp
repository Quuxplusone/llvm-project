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

#include <exception>
#include <cassert>

#include "test_macros.h"

struct A
{
    static int constructed;
    int data_;

    A(int data = 0) : data_(data) {++constructed;}
    ~A() {--constructed;}
    A(const A& a) : data_(a.data_) {++constructed;}
};

int A::constructed = 0;

struct Abstract {
    explicit Abstract() = default;
    Abstract(const Abstract&) = default;
    virtual int f() const = 0;
    virtual ~Abstract() = default;
};
struct Concrete : Abstract {
    int f() const override { return 42; };
};
struct Incomplete;

int main(int, char**)
{
    {
        std::exception_ptr p = std::make_exception_ptr(A(5));
        int cted = A::constructed;
#ifdef _LIBCPP_ABI_MICROSOFT
        // On Windows exception_ptr copies the exception
        assert(cted == 2);
#else
        assert(cted == 1);
#endif
        ASSERT_SAME_TYPE(decltype(std::try_cast<A>(p)), const A*);
        ASSERT_SAME_TYPE(decltype(std::try_cast<A>(nullptr)), const A*);
        const auto *pa = std::try_cast<A>(p);
        // The in-flight exception object is not copied
        assert(A::constructed == cted);
        const auto *pb = std::try_cast<A>(p);
        assert(A::constructed == cted);
        // The in-flight exception object's address doesn't change
        assert(pa == pb);
        assert(pa->data_ == 5);
    }
    assert(A::constructed == 0);

    {
        std::exception_ptr p;
        assert(std::try_cast<A>(p) == nullptr);
        assert(std::try_cast<A>(nullptr) == nullptr);
    }

    // Try modifying an object in flight
    {
        try {
            try {
                throw A(1);
            } catch (const A& a) {
                std::exception_ptr p = std::current_exception();
                const auto *pa = std::try_cast<A>(p);
                assert(pa == &a);
                assert(pa->data_ == 1);
                const_cast<A*>(pa)->data_ = 2;
                std::rethrow_exception(p);
                assert(false);
            }
        } catch (const A& a2) {
            assert(a2.data_ == 2);
        }
        assert(A::constructed == 0);
    }

    // Try catching some exotic types
    {
        std::exception_ptr p = std::make_exception_ptr(1);
        assert(std::try_cast<int>(p) != nullptr);
        assert(*std::try_cast<int>(p) == 1);
        assert(std::try_cast<long>(p) == nullptr);
        assert(std::try_cast<unsigned int>(p) == nullptr);

        p = std::make_exception_ptr(Concrete());
        assert(std::try_cast<Abstract>(p) != nullptr);
        assert(std::try_cast<Abstract>(p)->f() == 42);
    }
    return 0;
}
