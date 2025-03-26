// RUN: %clang_cc1 -std=c++14 -verify -DTEST_LINUX %s
// RUN: %clang_cc1 -std=c++23 -verify -DTEST_LINUX %s
// RUN: %clang_cc1 -std=c++14 -verify -DTEST_DARWIN %s
// RUN: %clang_cc1 -std=c++23 -verify -DTEST_DARWIN %s
// RUN: %clang_cc1 -std=c++14 -verify -DTEST_MSWIN %s
// RUN: %clang_cc1 -std=c++23 -verify -DTEST_MSWIN %s

#if defined(TEST_LINUX)
 extern "C" void __assert_fail(const char*, const char*, unsigned, const char*);
 #define assert(cond) \
   ((cond) ? (void)0 : __assert_fail(#cond, __FILE__, __LINE__, __func__))
#elif defined(TEST_DARWIN)
 void __assert_rtn(const char *, const char *, int, const char *);
 #define assert(cond) \
     (__builtin_expect(!(cond), 0) ? __assert_rtn(__func__, __FILE__, __LINE__, #cond) : (void)0)
#elif defined(TEST_MSWIN)
 void /*__cdecl*/ _wassert(const wchar_t*, const wchar_t*, unsigned);
 #define _CRT_WIDE_(s) L ## s
 #define _CRT_WIDE(s) _CRT_WIDE_(s)
 #define assert(cond) \
     (void)((!!(cond)) || (_wassert(_CRT_WIDE(#cond), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0))
#endif

constexpr int square(int x) {
  int result = x * x;
  assert(result == 42); // expected-note {{assertion failed during constant evaluation}}
  return result;
}

constexpr int val = square(2);
// expected-error@-1 {{constexpr variable 'val' must be initialized by a constant expression}}
// expected-note@-2 {{in call to 'square(2)'}}
