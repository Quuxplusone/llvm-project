// RUN: %clang_cc1 -std=c++17 -verify %s
// expected-no-diagnostics

#if __has_embed("Inputs/embed-100bytes.bin") != 1
 #error "not reached"
#endif

#if __has_embed("Inputs/embed-100bytes.bin" limit(42)) != 1
 #error "not reached"
#endif

#if __has_embed("Inputs/embed-100bytes.bin" limit(100)) != 1
 #error "not reached"
#endif

#if __has_embed("Inputs/embed-100bytes.bin" limit(107)) != 1
 #error "not reached"
#endif

#if __has_embed("Inputs/embed-empty.bin") != 2
 #error "not reached"
#endif

#if __has_embed("Inputs/embed-empty.bin" limit(0)) != 2
 #error "not reached"
#endif

#if __has_embed("Inputs/embed-100bytes.bin" limit(0)) != 2
 #error "not reached"
#endif
