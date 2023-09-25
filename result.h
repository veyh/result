#ifndef __result_h__
#define __result_h__

//
// Based on https://doc.rust-lang.org/std/result/enum.Result.html
//

#include <stdbool.h>

//
// You can use the _t suffix (and typedef) to create anonymous structs when you
// don't need forward declarations. Otherwise, use the _d suffix to create
// named structs.
//
// struct anon_s {
//   result_t(bool, int) foo;
//   result_t(size_t, int) bar;
// };
//
// struct named_s;
// struct named_s result_d(size_t, int);
//

#define result_t(_type, _err_type) \
  struct result_d(_type, _err_type)

#define result_d(_type, _err_type) { \
  union { _type ok; _err_type err; } body; \
  union { bool is_ok; } header; \
}

// The intention with padding is to avoid any warnings from -Wpadded and allow
// comparing via memcmp(3).

#define result_padded_t(_type, _err_type) \
  struct result_padded_d(_type, _err_type)

#define result_padded_d(_type, _err_type) { \
  union { void *pad; bool is_ok; } header; \
  union { void *pad; _type ok; _err_type err; } body; \
}

// There's also a packed variant if you really need it...

#define result_packed_t(_type, _err_type) \
  struct result_packed_d(_type, _err_type)

#define result_packed_d(_type, _err_type) __attribute__((packed)) { \
  union { _type ok; _err_type err; } body; \
  union { bool is_ok; } header; \
}

#define result_init_ok(_value) \
  { .header.is_ok = true, .body.ok = (_value) }

#define result_init_err(_err) \
  { .header.is_ok = false, .body.err = (_err) }

#define result_set_ok(_result, _value) ( \
  (_result).header.is_ok = true, \
  (_result).body.ok = (_value) \
)

#define result_set_err(_result, _err) ( \
  (_result).header.is_ok = false, \
  (_result).body.err = (_err) \
)

#define result_is_ok(_result) \
  (_result).header.is_ok

#define result_is_err(_result) \
  (!((_result).header.is_ok))

#define result_and(_a, _b) ( \
  result_is_ok(_a) \
    ? (_b) \
    : (_a) \
)

#define result_or(_a, _b) ( \
  result_is_ok(_a) \
    ? (_a) \
    : (_b) \
)

#define result_unwrap_unchecked(_result) \
  (_result).body.ok

#define result_unwrap_err_unchecked(_result) \
  (_result).body.err

#define result_unwrap_or(_result, _default_value) ( \
  result_is_ok(_result) \
    ? result_unwrap_unchecked(_result) \
    : (_default_value) \
)

#define result_unwrap_or_else_ptr(_result) \
  result_is_ok(_result) \
    ? &result_unwrap_unchecked(_result) \
    : NULL; \
  \
  for (bool ran = false; !ran && result_is_err(_result); ran = true)

//
// Everything below uses GNU extensions. Supported by GCC and clang.
//

#define result_ok(_result, _value) \
  (__typeof(_result)) { .header.is_ok = true, .body.ok = (_value) }

#define result_err(_result, _err) \
  (__typeof(_result)) { .header.is_ok = false, .body.err = (_err) }

#define result_unwrap_or_else(_result) \
  result_is_ok(_result) \
    ? result_unwrap_unchecked(_result) \
    : result_unwrap_unchecked(result_zero(_result)); \
  \
  for (bool ran = false; !ran && result_is_err(_result); ran = true)

#define result_zero(_result) ((__typeof(_result)) { 0 })

// result_with_{ok,err} is sort of an `if let Ok(value)` / `if let Err(value)`
// equivalent.
//
// NOTE: Because this declares a variable in the original scope to support an
// optional else block, you should make sure to have warnings enabled for
// variable shadowing to avoid unintended behavior (-Wshadow).
//
// You can of course scope this yourself but I admit it looks a bit funny if
// you omit the extra indentation:
//
//   {result_with_ok(res, value) {
//      printf("the value is %d\n", value);
//   }
//
//   else {
//     printf("there was no value\n");
//   }}

#define result_with_ok(_result, _variable) \
  __typeof((_result).body.ok) _variable = (_result).body.ok; \
  \
  if (result_is_ok(_result))

#define result_with_err(_result, _variable) \
  __typeof((_result).body.err) _variable = (_result).body.err; \
  \
  if (result_is_err(_result))

//
// Scoped versions of the above.
//

#define result_scoped_with_ok(_result, _variable, _block) { \
  __typeof((_result).body.ok) _variable = (_result).body.ok; \
  \
  if (result_is_ok(_result)) _block; \
}

#define result_scoped_with_err(_result, _variable, _block) { \
  __typeof((_result).body.err) _variable = (_result).body.err; \
  \
  if (result_is_err(_result)) _block; \
}

#define result_scoped_with_ok_or_else(_result, _variable, _block, _else_block) { \
  __typeof((_result).body.ok) _variable = (_result).body.ok; \
  \
  if (result_is_ok(_result)) _block; \
  else _else_block; \
}

#define result_scoped_with_err_or_else(_result, _variable, _block, _else_block) { \
  __typeof((_result).body.err) _variable = (_result).body.err; \
  \
  if (result_is_err(_result)) _block; \
  else _else_block; \
}

#endif // __result_h__
