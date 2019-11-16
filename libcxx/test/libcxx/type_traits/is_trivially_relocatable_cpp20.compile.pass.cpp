//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: c++98, c++03, c++11, c++14, c++17
//
// <type_traits>
//
// Test that many standard types are trivially relocatable.

#include <barrier>
#include <latch>
#include <semaphore>
#include <span>
#include <type_traits>


struct T { T(); T(const T&); ~T(); };
struct R { };
struct SM { void lock_shared(); void unlock_shared(); ~SM(); };

struct K {
    T t;
    bool operator==(const K&) const;
    bool operator< (const K&) const;
    struct Cmp {
        Cmp(); Cmp(const Cmp&); ~Cmp();
        bool operator()(const K&, const K&) const;
    };
};
template<> struct std::hash<K> { size_t operator()(const K&) const; };

//static const bool NotDebug =
//#ifdef _LIBCPP_ENABLE_DEBUG_MODE
//    false;
//#else
//    true;
//#endif

// Define the P1144 name for convenience.
#define is_trivially_relocatable __libcpp_is_trivially_relocatable

static_assert(std::is_trivially_relocatable<R>::value, "");
static_assert(!std::is_trivially_relocatable<T>::value, "");
static_assert(std::is_trivially_relocatable<std::span<T>>::value, "");

// The following types are not move-constructible at all.
static_assert(!std::is_trivially_relocatable<std::barrier<>>::value, "");
static_assert(!std::is_trivially_relocatable<std::latch>::value, "");
static_assert(!std::is_trivially_relocatable<std::counting_semaphore<3>>::value, "");
static_assert(!std::is_trivially_relocatable<std::binary_semaphore>::value, "");
