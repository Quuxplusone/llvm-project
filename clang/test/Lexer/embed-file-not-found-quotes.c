// RUN: %clang_cc1 %s -verify

char array[] = {

#embed "nonexistent-file-for-embed.txt"
// expected-error@-1{{cannot open file 'nonexistent-file-for-embed.txt': could not find the specified file}}

};
