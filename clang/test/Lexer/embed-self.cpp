// RUN: %clang_cc1 -std=c++11 -verify %s
// RUN: %clang_cc1 -std=c++20 -verify %s
// expected-no-diagnostics

#if __has_embed(__FILE__)
 // reached
#else
 #error "not reached"
#endif

#if __has_embed(__FILE__ limit(42))
 // reached
#else
 #error "not reached"
#endif

constexpr char myself[] = {
  #embed __FILE__
};
static_assert(myself[0] == '/', "");
static_assert(myself[1] == '/', "");
static_assert(myself[2] == ' ', "");
static_assert(myself[3] == 'R', "");
static_assert(myself[4] == 'U', "");
