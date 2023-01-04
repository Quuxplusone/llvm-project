// RUN: %clang_cc1 %s -verify

char array[] = {

#embed
// expected-error@-1{{expected "FILENAME" or <FILENAME>}}

#embed <
// expected-error@-1{{expected '>'}}
// expected-note@-2{{to match this '<'}}

#embed "
// expected-error@-1{{expected "FILENAME" or <FILENAME>}}
// expected-warning@-2{{missing terminating '"' character}}

#embed noquotes.txt
// expected-error@-1{{expected "FILENAME" or <FILENAME>}}

#embed "file-will-not-be-statted" 42
// expected-error@-1{{expected identifier}}

#embed "file-will-not-be-statted" limit
// expected-error@-1{{missing '(' after 'limit'}}

#embed "file-will-not-be-statted" limit 42
// expected-error@-1{{missing '(' after 'limit'}}

#embed "file-will-not-be-statted" limit(
// expected-error@-1{{expected value in expression}}

#embed "file-will-not-be-statted" limit(unrecognized
// expected-error@-1{{expected ')' in preprocessor expression}}

#embed "file-will-not-be-statted" limit(42
// expected-error@-1{{expected ')' in preprocessor expression}}

#embed "file-will-not-be-statted" limit([
// expected-error@-1{{invalid token at start of a preprocessor expression}}

#embed "file-will-not-be-statted" limit([)
// expected-error@-1{{invalid token at start of a preprocessor expression}}

#embed "file-will-not-be-statted" limit(1/0)
// expected-error@-1{{division by zero in preprocessor expression}}

#embed "file-will-not-be-statted" prefix
// expected-error@-1{{missing '(' after 'prefix'}}

#embed "file-will-not-be-statted" prefix 42
// expected-error@-1{{missing '(' after 'prefix'}}

#embed "file-will-not-be-statted" suffix
// expected-error@-1{{missing '(' after 'suffix'}}

#embed "file-will-not-be-statted" suffix 42
// expected-error@-1{{missing '(' after 'suffix'}}

#embed "file-will-not-be-statted" if_empty
// expected-error@-1{{missing '(' after 'if_empty'}}

#embed "file-will-not-be-statted" if_empty 42
// expected-error@-1{{missing '(' after 'if_empty'}}

#embed "file-will-not-be-statted" unrecognized::
// expected-error@-1{{expected identifier}}

#embed "file-will-not-be-statted" unrecognized(
// expected-error@-1{{expected ')'}}

#embed "file-will-not-be-statted" unrecognized([
// expected-error@-1{{expected ']'}}

#embed "file-will-not-be-statted" unrecognized([garbage)
// expected-error@-1{{expected ']'}}

};
