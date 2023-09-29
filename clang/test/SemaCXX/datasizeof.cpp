// RUN: %clang_cc1 -fsyntax-only -triple x86_64-linux-gnu -verify %s

// expected-no-diagnostics

#if !__has_feature(datasizeof)
#  error "Expected datasizeof feature"
#endif

struct HasPadding {
  int i;
  char c;
};

struct HasUsablePadding {
  int i;
  char c;

  HasUsablePadding() {}
};

static_assert(__datasizeof(int) == 4);
static_assert(__datasizeof(HasPadding) == 8);
static_assert(__datasizeof(HasUsablePadding) == 5);

static_assert([] {
  int* p = nullptr;
  HasPadding* p2 = nullptr;
  HasUsablePadding* p3 = nullptr;
  static_assert(__datasizeof(*p) == 4);
  static_assert(__datasizeof(*p2) == 8);
  static_assert(__datasizeof(*p3) == 5);

  return true;
}());
