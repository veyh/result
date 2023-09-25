#ifndef __w_stringify_h__
#define __w_stringify_h__

// Use w_stringify to convert numbers to strings. Example:
//
//   #define FOO 1
//   #define BAR 2
//   #define FOOBAR w_stringify(FOO) "." w_stringify(BAR)
//
//   FOOBAR expands like so
//
//   "1" "." "2"
//
//   and because constant strings are concatenated, the result is
//
//   "1.2"

#define w_stringify_impl(x) #x
#define w_stringify(x) w_stringify_impl(x)

#endif // __w_stringify_h__
