// RUN: %clang_cc1 -std=c++17 -verify %s
// expected-no-diagnostics

#if __has_embed("Inputs/embed-100bytes.bin" prefix(42,)) != 1
 #error "not reached"
#endif

#if __has_embed("Inputs/embed-100bytes.bin" suffix(,43)) != 1
 #error "not reached"
#endif

#if __has_embed("Inputs/embed-100bytes.bin" prefix(42,) suffix()) != 1
 #error "not reached"
#endif

#if __has_embed("Inputs/embed-100bytes.bin" prefix() suffix(,43)) != 1
 #error "not reached"
#endif

#if __has_embed("Inputs/embed-100bytes.bin" prefix(42,) suffix(,43)) != 1
 #error "not reached"
#endif

#if __has_embed("Inputs/embed-100bytes.bin" limit(36) prefix(42,) suffix(,43)) != 1
 #error "not reached"
#endif

#if __has_embed("Inputs/embed-100bytes.bin" limit(1) prefix(42,) suffix(,43)) != 1
 #error "not reached"
#endif

// 'prefix' and 'suffix' are both ignored if the resource is empty.

#if __has_embed("Inputs/embed-100bytes.bin" limit(0) prefix(42,) suffix(,43)) != 2
 #error "not reached"
#endif

#if __has_embed("Inputs/embed-empty.bin" prefix(42,) suffix(,43)) != 2
 #error "not reached"
#endif

#if __has_embed("Inputs/embed-empty.bin" prefix(42,) suffix(,43) limit(1)) != 2
 #error "not reached"
#endif
