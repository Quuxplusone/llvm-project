// RUN: %clang_cc1 %s -verify

#if __has_embed
 #error "not reached"
#endif
// expected-error@-3{{missing '(' after '__has_embed'}}
// expected-error@-4{{expected value in expression}}

#if __has_embed(
 #error "not reached"
#endif
// expected-error@-3{{expected "FILENAME" or <FILENAME>}}
// expected-error@-4{{expected value in expression}}

#if __has_embed <
 #error "not reached"
#endif
// expected-error@-3{{expected '>'}}
// expected-note@-4{{to match this '<'}}
// expected-error@-5{{expected value in expression}}

#if __has_embed("
 #error "not reached"
#endif
// expected-warning@-3{{missing terminating '"' character}}
// expected-error@-4{{expected "FILENAME" or <FILENAME>}}
// expected-error@-5{{invalid token at start of a preprocessor expression}}

#if __has_embed(noquotes.txt
 #error "not reached"
#endif
// expected-error@-3{{expected "FILENAME" or <FILENAME>}}
// expected-error@-4{{token is not a valid binary operator in a preprocessor subexpression}}

#if __has_embed("file-will-not-be-statted" 42)
 #error "unfortunately reached"
#endif
// expected-error@-3{{expected identifier}}
// expected-error@-4{{expected end of line in preprocessor expression}}
// expected-error@-4{{unfortunately reached}}

#if __has_embed("file-will-not-be-statted" limit
 #error "not reached"
#endif
// expected-error@-3{{missing '(' after 'limit'}}
// expected-error@-4{{expected value in expression}}

#if __has_embed("file-will-not-be-statted" limit)
 #error "not reached"
#endif
// expected-error@-3{{missing '(' after 'limit'}}

#if __has_embed("file-will-not-be-statted" limit 42)
 #error "unfortunately reached"
#endif
// expected-error@-3{{missing '(' after 'limit'}}
// expected-error@-4{{expected end of line in preprocessor expression}}
// expected-error@-4{{unfortunately reached}}

#if __has_embed("file-will-not-be-statted" limit(
 #error "not reached"
#endif
// expected-error@-3{{expected value in expression}}
// expected-error@-4{{expected value in expression}}

#if __has_embed("file-will-not-be-statted" limit(unrecognized
 #error "not reached"
#endif
// expected-error@-3{{expected ')' in preprocessor expression}}
// expected-error@-4{{expected value in expression}}

#if __has_embed("file-will-not-be-statted" limit(42
 #error "not reached"
#endif
// expected-error@-3{{expected ')' in preprocessor expression}}
// expected-error@-4{{expected value in expression}}

#if __has_embed("file-will-not-be-statted" limit([
 #error "not reached"
#endif
// expected-error@-3{{invalid token at start of a preprocessor expression}}
// expected-error@-4{{invalid token at start of a preprocessor expression}}

#if __has_embed("file-will-not-be-statted" limit([))
 #error "not reached"
#endif
// expected-error@-3{{invalid token at start of a preprocessor expression}}
// expected-error@-4{{invalid token at start of a preprocessor expression}}

#if __has_embed("file-will-not-be-statted" limit(1/0))
 #error "not reached"
#endif
// expected-error@-3{{division by zero in preprocessor expression}}
// expected-error@-4{{expected end of line in preprocessor expression}}

#if __has_embed("file-will-not-be-statted" prefix
 #error "not reached"
#endif
// expected-error@-3{{missing '(' after 'prefix'}}
// expected-error@-4{{expected value in expression}}

#if __has_embed("file-will-not-be-statted" prefix)
 #error "not reached"
#endif
// expected-error@-3{{missing '(' after 'prefix'}}

#if __has_embed("file-will-not-be-statted" prefix 42)
 #error "unfortunately reached"
#endif
// expected-error@-3{{missing '(' after 'prefix'}}
// expected-error@-4{{expected end of line in preprocessor expression}}
// expected-error@-4{{unfortunately reached}}

#if __has_embed("file-will-not-be-statted" suffix
 #error "not reached"
#endif
// expected-error@-3{{missing '(' after 'suffix'}}
// expected-error@-4{{expected value in expression}}

#if __has_embed("file-will-not-be-statted" suffix 42
 #error "unfortunately reached"
#endif
// expected-error@-3{{missing '(' after 'suffix'}}
// expected-error@-3{{unfortunately reached}}

#if __has_embed("file-will-not-be-statted" if_empty
 #error "not reached"
#endif
// expected-error@-3{{missing '(' after 'if_empty'}}
// expected-error@-4{{expected value in expression}}

#if __has_embed("file-will-not-be-statted" if_empty 42
 #error "unfortunately reached"
#endif
// expected-error@-3{{missing '(' after 'if_empty'}}
// expected-error@-3{{unfortunately reached}}

#if __has_embed("file-will-not-be-statted" unrecognized::
 #error "not reached"
#endif
// expected-error@-3{{expected identifier}}
// expected-error@-4{{expected value in expression}}

#if __has_embed("file-will-not-be-statted" unrecognized::)
 #error "not reached"
#endif
// expected-error@-3{{expected identifier}}

#if __has_embed("file-will-not-be-statted" unrecognized(
 #error "not reached"
#endif
// expected-error@-3{{expected ')'}}
// expected-error@-4{{expected value in expression}}

#if __has_embed("file-will-not-be-statted" unrecognized([
 #error "not reached"
#endif
// expected-error@-3{{expected ']'}}
// expected-error@-4{{expected value in expression}}

#if __has_embed("file-will-not-be-statted" unrecognized([garbage))
 #error "not reached"
#endif
// expected-error@-3{{expected ']'}}
// expected-error@-4{{expected value in expression}}
