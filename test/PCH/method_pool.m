// Test this without pch.
// RUN: %clang_cc1 -include %S/method_pool.h -fsyntax-only -verify %s

// Test with pch.
// RUN: %clang_cc1 -x objective-c -emit-pch -o %t %S/method_pool.h
// RUN: %clang_cc1 -include-pch %t -fsyntax-only -verify %s 

int message_id(id x) {
   return [x instMethod:17]; // expected-warning{{multiple methods}}
}





/* Whitespace below is significant */
/* expected-note{{using}} */



/* expected-note{{also}} */
