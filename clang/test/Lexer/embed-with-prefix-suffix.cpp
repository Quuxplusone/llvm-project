// RUN: %clang_cc1 -std=c++17 -verify %s
// expected-no-diagnostics

constexpr unsigned char expected[] = {
  42, 0x87, 0x03, 0x80, 0xdf, 0xd5, 0x86, 0x76, 0x00, 0x94, 0x25, 0x0f, 0xbb,
  0x32, 0x15, 0x8d, 0xa7, 0x0e, 0x15, 0x3d, 0xb7, 0x5a, 0x74, 0xac, 0xa1,
  0x14, 0x9a, 0xf1, 0x7b, 0x24, 0x31, 0x09, 0xf8, 0x2d, 0x05, 0xec, 0xd0,
  0x83, 0x1e, 0xf8, 0x59, 0xb1, 0xe9, 0x00, 0xe4, 0x15, 0x1d, 0x09, 0x8a,
  0x61, 0x44, 0x5b, 0x5c, 0x61, 0xea, 0xe5, 0xe2, 0xb9, 0x17, 0x2d, 0x82,
  0x17, 0xf3, 0xbf, 0xab, 0x9d, 0x40, 0x0c, 0x57, 0x51, 0x9d, 0xdf, 0x76,
  0x6f, 0x93, 0xba, 0xd1, 0x2e, 0x90, 0xb3, 0x9b, 0xc4, 0x5c, 0xac, 0x3c,
  0x13, 0xc7, 0xc3, 0x3a, 0xc5, 0x47, 0x11, 0xf4, 0xf6, 0xe5, 0x34, 0x7d,
  0x4a, 0xa2, 0x12, 0x1d, 43
};
constexpr unsigned char expected36[] = {
  42, 0x87, 0x03, 0x80, 0xdf, 0xd5, 0x86, 0x76, 0x00, 0x94, 0x25, 0x0f, 0xbb,
  0x32, 0x15, 0x8d, 0xa7, 0x0e, 0x15, 0x3d, 0xb7, 0x5a, 0x74, 0xac, 0xa1,
  0x14, 0x9a, 0xf1, 0x7b, 0x24, 0x31, 0x09, 0xf8, 0x2d, 0x05, 0xec, 0xd0, 43
};
constexpr unsigned char expected1[] = { 42, 0x87, 43 };
constexpr unsigned char expected_h[] = { 42, 0x00, 43 };

template<class T, int PN, class U>
constexpr bool std_equal(const T (&p)[PN], const U *q, int qn) {
  if (PN != qn)
    return false;
  for (int i=0; i < PN; ++i)
    if (p[i] != q[i])
      return false;
  return true;
}

constexpr bool is_empty_arg_list() { return true; }

constexpr unsigned char a[] = {
  #embed "Inputs/embed-100bytes.bin" prefix(42,)
};
static_assert(std_equal(a, expected, 101));

constexpr unsigned char b[] = {
  #embed "Inputs/embed-100bytes.bin" suffix(,43)
};
static_assert(std_equal(b, expected+1, 101));

constexpr unsigned char a2[] = {
  #embed "Inputs/embed-100bytes.bin" prefix(42,) suffix()
};
static_assert(std_equal(a2, expected, 101));

constexpr unsigned char b2[] = {
  #embed "Inputs/embed-100bytes.bin" prefix() suffix(,43)
};
static_assert(std_equal(b2, expected+1, 101));

constexpr unsigned char c[] = {
  #embed "Inputs/embed-100bytes.bin" prefix(42,) suffix(,43)
};
static_assert(std_equal(c, expected, 102));

constexpr unsigned char d[] = {
  #embed "Inputs/embed-100bytes.bin" limit(36) prefix(42,) suffix(,43)
};
static_assert(std_equal(d, expected36, 38));

constexpr unsigned char e[] = {
  #embed "Inputs/embed-100bytes.bin" limit(1) prefix(42,) suffix(,43)
};
static_assert(std_equal(e, expected1, 3));

// 'prefix' and 'suffix' are both ignored if the resource is empty.

static_assert(is_empty_arg_list(
  #embed "Inputs/embed-100bytes.bin" limit(0) prefix(42,) suffix(,43)
));

static_assert(is_empty_arg_list(
  #embed "Inputs/embed-empty.bin" prefix(42,) suffix(,43)
));

static_assert(is_empty_arg_list(
  #embed "Inputs/embed-empty.bin" prefix(42,) suffix(,43) limit(1)
));
