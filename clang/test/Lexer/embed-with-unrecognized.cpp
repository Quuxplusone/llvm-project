// RUN: %clang_cc1 -std=c++17 -verify %s

constexpr unsigned char expected[] = {
  0x87, 0x03, 0x80, 0xdf, 0xd5, 0x86, 0x76, 0x00, 0x94, 0x25, 0x0f, 0xbb,
  0x32, 0x15, 0x8d, 0xa7, 0x0e, 0x15, 0x3d, 0xb7, 0x5a, 0x74, 0xac, 0xa1,
  0x14, 0x9a, 0xf1, 0x7b, 0x24, 0x31, 0x09, 0xf8, 0x2d, 0x05, 0xec, 0xd0,
  0x83, 0x1e, 0xf8, 0x59, 0xb1, 0xe9, 0x00, 0xe4, 0x15, 0x1d, 0x09, 0x8a,
  0x61, 0x44, 0x5b, 0x5c, 0x61, 0xea, 0xe5, 0xe2, 0xb9, 0x17, 0x2d, 0x82,
  0x17, 0xf3, 0xbf, 0xab, 0x9d, 0x40, 0x0c, 0x57, 0x51, 0x9d, 0xdf, 0x76,
  0x6f, 0x93, 0xba, 0xd1, 0x2e, 0x90, 0xb3, 0x9b, 0xc4, 0x5c, 0xac, 0x3c,
  0x13, 0xc7, 0xc3, 0x3a, 0xc5, 0x47, 0x11, 0xf4, 0xf6, 0xe5, 0x34, 0x7d,
  0x4a, 0xa2, 0x12, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

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
  #embed "Inputs/embed-100bytes.bin" unrecognized
  // expected-warning@-1{{unknown #embed parameter 'unrecognized' ignored}}
};
static_assert(std_equal(a, expected, 100));

constexpr unsigned char b[] = {
  #embed "Inputs/embed-100bytes.bin" unrecognized()
  // expected-warning@-1{{unknown #embed parameter 'unrecognized' ignored}}
};
static_assert(std_equal(b, expected, 100));

constexpr unsigned char c[] = {
  #embed "Inputs/embed-100bytes.bin" unrecognized(token soup, ( [ { ( ) [ ] } ] ) braces)
  // expected-warning@-1{{unknown #embed parameter 'unrecognized' ignored}}
};
static_assert(std_equal(c, expected, 100));

constexpr unsigned char d[] = {
  #embed "Inputs/embed-100bytes.bin" unrecognized(())
  // expected-warning@-1{{unknown #embed parameter 'unrecognized' ignored}}
};
static_assert(std_equal(d, expected, 100));

constexpr unsigned char e[] = {
  #embed "Inputs/embed-100bytes.bin" unrecognized([])
  // expected-warning@-1{{unknown #embed parameter 'unrecognized' ignored}}
};
static_assert(std_equal(e, expected, 100));

constexpr unsigned char f[] = {
  #embed "Inputs/embed-100bytes.bin" unrecognized({})
  // expected-warning@-1{{unknown #embed parameter 'unrecognized' ignored}}
};
static_assert(std_equal(f, expected, 100));


// An unrecognized parameter should warn, but not interfere with other parameters.

constexpr unsigned char g[] = {
  #embed "Inputs/embed-100bytes.bin" limit(42) unrecognized
  // expected-warning@-1{{unknown #embed parameter 'unrecognized' ignored}}
};
static_assert(std_equal(g, expected, 42));

constexpr unsigned char h[] = {
  #embed "Inputs/embed-100bytes.bin" limit(42) unrecognized(())
  // expected-warning@-1{{unknown #embed parameter 'unrecognized' ignored}}
};
static_assert(std_equal(h, expected, 42));

constexpr unsigned char i[] = {
  #embed "Inputs/embed-100bytes.bin" unrecognized limit(42)
  // expected-warning@-1{{unknown #embed parameter 'unrecognized' ignored}}
};
static_assert(std_equal(i, expected, 42));

constexpr unsigned char j[] = {
  #embed "Inputs/embed-100bytes.bin" unrecognized(()) limit(42)
  // expected-warning@-1{{unknown #embed parameter 'unrecognized' ignored}}
};
static_assert(std_equal(j, expected, 42));
