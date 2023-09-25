#include "core/defs.h"
#include "core/w_stringify.h"
#include "result.h"

/*sublime-c-static-fn-hoist-start*/
static void test_inits_ok_directly(void **ts);
static void test_inits_err_directly(void **ts);
static void test_updates_from_value_to_value(void **ts);
static void test_updates_from_err_to_err(void **ts);
static void test_updates_from_value_to_err(void **ts);
static void test_updates_from_err_to_value(void **ts);
static void test_updates_from_value_to_value_via_set(void **ts);
static void test_updates_from_err_to_err_via_set(void **ts);
static void test_updates_from_value_to_err_via_set(void **ts);
static void test_updates_from_err_to_value_via_set(void **ts);
static void test_and_returns_the_second_result_when_first_is_ok(void **ts);
static void test_and_returns_the_first_result_when_its_an_error(void **ts);
static void test_and_returns_the_first_result_when_both_are_errors(void **ts);
static void test_and_returns_the_second_result_when_both_are_ok(void **ts);
static void test_or_with_ok_and_err_returns_ok(void **ts);
static void test_or_with_err_and_ok_returns_ok(void **ts);
static void test_or_with_err_and_err_returns_the_second_err(void **ts);
static void test_or_with_ok_and_ok_returns_the_first_ok(void **ts);
static void test_unwrap_or_for_none_returns_default_value(void **ts);
static void test_unwrap_or_for_none_returns_original_value(void **ts);
static void test_result_with_ok_for_err_does_nothing(void **ts);
static void test_result_with_ok_for_err_runs_else_block(void **ts);
static void test_result_with_ok_for_ok_runs_block_with_value(void **ts);
static void test_result_with_ok_for_ok_does_not_run_else_block(void **ts);
static void test_result_with_err_for_ok_does_nothing(void **ts);
static void test_result_with_err_for_ok_runs_else_block(void **ts);
static void test_result_with_err_for_err_runs_block_with_value(void **ts);
static void test_result_with_err_for_err_does_not_run_else_block(void **ts);
static void test_unwrap_or_else_with_ok_returns_the_value(void **ts);
static void test_unwrap_or_else_with_err_returns_zeroed_value_and_runs_the_block(void **ts);
static void test_unwrap_or_else_with_ok_and_missing_else_block_returns_the_value(void **ts);
static void test_unwrap_or_else_with_err_and_missing_else_block_returns_zeroed_value(void **ts);
static void test_pads_to_multiple_of_pointer_size(void **ts);
static void test_allows_forward_declaration(void **ts);
static void test_result_scoped_with_ok_for_err_does_nothing(void **ts);
static void test_result_scoped_with_ok_for_ok_runs_block_with_value(void **ts);
static void test_result_scoped_with_ok_or_else_for_err_does_not_run_block(void **ts);
static void test_result_scoped_with_ok_or_else_for_err_runs_else_block(void **ts);
static void test_result_scoped_with_ok_or_else_for_ok_runs_block_with_value(void **ts);
static void test_result_scoped_with_ok_or_else_for_ok_does_not_run_else_block(void **ts);
/*sublime-c-static-fn-hoist-end*/

#define assert_ok(_result, _value) \
  assert_true(result_is_ok(_result)); \
  assert_false(result_is_err(_result)); \
  assert_int_equal((_value), result_unwrap_unchecked(_result)); \

#define assert_err(_result, _err) \
  assert_false(result_is_ok(_result)); \
  assert_true(result_is_err(_result)); \
  assert_int_equal((_err), result_unwrap_err_unchecked(_result)); \

static void test_inits_ok_directly(void **ts) {
  result_t(int, int) res = result_init_ok(111);

  assert_ok(res, 111);
}

static void test_inits_err_directly(void **ts) {
  result_t(int, int) res = result_init_err(-111);

  assert_err(res, -111);
}

static void test_updates_from_value_to_value(void **ts) {
  result_t(int, int) res = result_init_ok(111);
  res = result_ok(res, 222);

  assert_ok(res, 222);
}

static void test_updates_from_err_to_err(void **ts) {
  result_t(int, int) res = result_init_err(-111);
  res = result_err(res, -222);

  assert_err(res, -222);
}

static void test_updates_from_value_to_err(void **ts) {
  result_t(int, int) res = result_init_ok(111);
  res = result_err(res, -222);

  assert_err(res, -222);
}

static void test_updates_from_err_to_value(void **ts) {
  result_t(int, int) res = result_init_err(-111);
  res = result_ok(res, 222);

  assert_ok(res, 222);
}

static void test_updates_from_value_to_value_via_set(void **ts) {
  result_t(int, int) res = result_init_ok(111);
  result_set_ok(res, 222);

  assert_ok(res, 222);
}

static void test_updates_from_err_to_err_via_set(void **ts) {
  result_t(int, int) res = result_init_err(-111);
  result_set_err(res, -222);

  assert_err(res, -222);
}

static void test_updates_from_value_to_err_via_set(void **ts) {
  result_t(int, int) res = result_init_ok(111);
  result_set_err(res, -222);

  assert_err(res, -222);
}

static void test_updates_from_err_to_value_via_set(void **ts) {
  result_t(int, int) res = result_init_err(-111);
  result_set_ok(res, 222);

  assert_ok(res, 222);
}

static void test_and_returns_the_second_result_when_first_is_ok(void **ts) {
  typedef result_t(int, int) result_int_int_t;

  result_int_int_t a = result_init_ok(111);
  result_int_int_t b_ok = result_init_ok(222);
  result_int_int_t b_err = result_init_err(333);

  assert_ok(result_and(a, b_ok), 222);
  assert_err(result_and(a, b_err), 333);
}

static void test_and_returns_the_first_result_when_its_an_error(void **ts) {
  typedef result_t(int, int) result_int_int_t;

  result_int_int_t a = result_init_err(111);
  result_int_int_t b = result_init_ok(222);

  assert_err(result_and(a, b), 111);
}

static void test_and_returns_the_first_result_when_both_are_errors(void **ts) {
  typedef result_t(int, int) result_int_int_t;

  result_int_int_t a = result_init_err(111);
  result_int_int_t b = result_init_err(222);

  assert_err(result_and(a, b), 111);
}

static void test_and_returns_the_second_result_when_both_are_ok(void **ts) {
  typedef result_t(int, int) result_int_int_t;

  result_int_int_t a = result_init_ok(111);
  result_int_int_t b = result_init_ok(222);

  assert_ok(result_and(a, b), 222);
}

static void test_or_with_ok_and_err_returns_ok(void **ts) {
  typedef result_t(int, int) result_int_int_t;

  result_int_int_t a = result_init_ok(111);
  result_int_int_t b = result_init_err(222);

  assert_ok(result_or(a, b), 111);
}

static void test_or_with_err_and_ok_returns_ok(void **ts) {
  typedef result_t(int, int) result_int_int_t;

  result_int_int_t a = result_init_err(111);
  result_int_int_t b = result_init_ok(222);

  assert_ok(result_or(a, b), 222);
}

static void test_or_with_err_and_err_returns_the_second_err(void **ts) {
  typedef result_t(int, int) result_int_int_t;

  result_int_int_t a = result_init_err(111);
  result_int_int_t b = result_init_err(222);

  assert_err(result_or(a, b), 222);
}

static void test_or_with_ok_and_ok_returns_the_first_ok(void **ts) {
  typedef result_t(int, int) result_int_int_t;

  result_int_int_t a = result_init_ok(111);
  result_int_int_t b = result_init_ok(222);

  assert_ok(result_or(a, b), 111);
}

static void test_unwrap_or_for_none_returns_default_value(void **ts) {
  result_t(int, int) res = result_init_err(111);

  assert_int_equal(222, result_unwrap_or(res, 222));
}

static void test_unwrap_or_for_none_returns_original_value(void **ts) {
  result_t(int, int) res = result_init_ok(111);

  assert_int_equal(111, result_unwrap_or(res, 222));
}

static void test_result_with_ok_for_err_does_nothing(void **ts) {
  result_t(int, int) res = result_init_err(111);

  result_with_ok(res, value) {
    (void) value;
    fail();
  }
}

static void test_result_with_ok_for_err_runs_else_block(void **ts) {
  result_t(int, int)res = result_init_err(111);

  result_with_ok(res, value) {
    (void) value;
  }

  else {
    return;
  }

  fail();
}

static void test_result_with_ok_for_ok_runs_block_with_value(void **ts) {
  result_t(int, int) res = result_init_ok(111);

  int received_value = 0;

  result_with_ok(res, value) {
    received_value = value;
  }

  assert_int_equal(111, received_value);
}

static void test_result_with_ok_for_ok_does_not_run_else_block(void **ts) {
  result_t(int, int) res = result_init_ok(111);

  result_with_ok(res, value) {
    (void) value;
  }

  else {
    fail();
  }
}

static void test_result_with_err_for_ok_does_nothing(void **ts) {
  result_t(int, int) res = result_init_ok(111);

  result_with_err(res, value) {
    (void) value;
    fail();
  }
}

static void test_result_with_err_for_ok_runs_else_block(void **ts) {
  result_t(int, int)res = result_init_ok(111);

  result_with_err(res, value) {
    (void) value;
  }

  else {
    return;
  }

  fail();
}

static void test_result_with_err_for_err_runs_block_with_value(void **ts) {
  result_t(int, int) res = result_init_err(111);

  int received_value = 0;

  result_with_err(res, value) {
    received_value = value;
  }

  assert_int_equal(111, received_value);
}

static void test_result_with_err_for_err_does_not_run_else_block(void **ts) {
  result_t(int, int) res = result_init_err(111);

  result_with_err(res, value) {
    (void) value;
  }

  else {
    fail();
  }
}

static void test_unwrap_or_else_with_ok_returns_the_value(void **ts) {
  result_t(int, int) res = result_init_ok(111);

  int value = result_unwrap_or_else(res) {
    fail();
  }

  assert_int_equal(111, value);
}

static void test_unwrap_or_else_with_err_returns_zeroed_value_and_runs_the_block(void **ts) {
  result_t(int, int) res = result_init_err(111);

  bool block_was_run = false;

  int value = result_unwrap_or_else(res) {
    block_was_run = true;
  }

  assert_int_equal(0, value);
  assert_true(block_was_run);
}

static void test_unwrap_or_else_with_ok_and_missing_else_block_returns_the_value(void **ts) {
  // NOTE: While the missing else block is not really an intended use case, we
  // should still make sure nothing bad happens if it's left out.

  result_t(int, int) res = result_init_ok(111);

  int value = result_unwrap_or_else(res);
  int other_value = 222;

  assert_int_equal(111, value);
  assert_int_equal(222, other_value);
}

static void test_unwrap_or_else_with_err_and_missing_else_block_returns_zeroed_value(void **ts) {
  // NOTE: While the missing else block is not really an intended use case, we
  // should still make sure nothing bad happens if it's left out.

  result_t(int, int) res = result_init_err(111);

  int value = result_unwrap_or_else(res);
  int other_value = 222;

  assert_int_equal(0, value);
  assert_int_equal(222, other_value);
}

#define assert_size_is_multiple_of(_option, _expected) { \
  size_t size = sizeof(_option); \
  \
  if ((size % (_expected)) != 0) { \
    fail_msg(\
      "sizeof(%s) was expected to be a multiple of %zu but it wasn't (got %zu)", \
      w_stringify(_option), \
      _expected, \
      size \
    ); \
  } \
}

static void test_pads_to_multiple_of_pointer_size(void **ts) {
  assert_size_is_multiple_of(
    result_padded_t(uint8_t, uint8_t),
    sizeof(void *)
  );

  assert_size_is_multiple_of(
    result_padded_t(uint16_t, uint16_t),
    sizeof(void *)
  );

  assert_size_is_multiple_of(
    result_padded_t(uint32_t, uint32_t),
    sizeof(void *)
  );

  assert_size_is_multiple_of(
    result_padded_t(uint64_t, uint64_t),
    sizeof(void *)
  );

  assert_size_is_multiple_of(
    result_padded_t(void *, void *),
    sizeof(void *)
  );

  typedef result_padded_t(
    struct { char x[1]; },
    struct { char x[1]; }
  ) res_char_1;

  assert_size_is_multiple_of(res_char_1, sizeof(void *));

  typedef result_padded_t(
    struct { char x[2]; },
    struct { char x[2]; }
  ) res_char_2;

  assert_size_is_multiple_of(res_char_2, sizeof(void *));

  typedef result_padded_t(
    struct { char x[3]; },
    struct { char x[3]; }
  ) res_char_3;

  assert_size_is_multiple_of(res_char_3, sizeof(void *));

  typedef result_padded_t(
    struct { char x[4]; },
    struct { char x[4]; }
  ) res_char_4;

  assert_size_is_multiple_of(res_char_4, sizeof(void *));

  typedef result_padded_t(
    struct { char x[5]; },
    struct { char x[5]; }
  ) res_char_5;

  assert_size_is_multiple_of(res_char_5, sizeof(void *));

  typedef result_padded_t(
    struct { char x[6]; },
    struct { char x[6]; }
  ) res_char_6;

  assert_size_is_multiple_of(res_char_6, sizeof(void *));

  typedef result_padded_t(
    struct { char x[7]; },
    struct { char x[7]; }
  ) res_char_7;

  assert_size_is_multiple_of(res_char_7, sizeof(void *));

  typedef result_padded_t(
    struct { char x[8]; },
    struct { char x[8]; }
  ) res_char_8;

  assert_size_is_multiple_of(res_char_8, sizeof(void *));

  typedef result_padded_t(
    struct { char x[9]; },
    struct { char x[9]; }
  ) res_char_9;

  assert_size_is_multiple_of(res_char_9, sizeof(void *));

  typedef result_padded_t(
    struct { char x[10]; },
    struct { char x[10]; }
  ) res_char_10;

  assert_size_is_multiple_of(res_char_10, sizeof(void *));

  typedef result_padded_t(
    struct { char x[11]; },
    struct { char x[11]; }
  ) res_char_11;

  assert_size_is_multiple_of(res_char_11, sizeof(void *));

  typedef result_padded_t(
    struct { char x[12]; },
    struct { char x[12]; }
  ) res_char_12;

  assert_size_is_multiple_of(res_char_12, sizeof(void *));

  typedef result_padded_t(
    struct { char x[13]; },
    struct { char x[13]; }
  ) res_char_13;

  assert_size_is_multiple_of(res_char_13, sizeof(void *));

  typedef result_padded_t(
    struct { char x[14]; },
    struct { char x[14]; }
  ) res_char_14;

  assert_size_is_multiple_of(res_char_14, sizeof(void *));

  typedef result_padded_t(
    struct { char x[15]; },
    struct { char x[15]; }
  ) res_char_15;

  assert_size_is_multiple_of(res_char_15, sizeof(void *));

  typedef result_padded_t(
    struct { char x[16]; },
    struct { char x[16]; }
  ) res_char_16;

  assert_size_is_multiple_of(res_char_16, sizeof(void *));

  typedef result_padded_t(
    struct { char x[17]; },
    struct { char x[17]; }
  ) res_char_17;

  assert_size_is_multiple_of(res_char_17, sizeof(void *));
}

static void test_allows_forward_declaration(void **ts) {
  struct result_forward_decl_test_s;
  struct result_forward_decl_test_s result_d(int, int);

  w_unused struct result_forward_decl_test_s foo = result_init_ok(123);

  // no assertions because the point is to make sure the above compiles
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wgnu-statement-expression"

static void test_result_scoped_with_ok_for_err_does_nothing(void **ts) {
  result_t(int, int) res = result_init_err(111);

  result_scoped_with_ok(res, value, {
    (void) value;
    fail();
  });
}

static void test_result_scoped_with_ok_for_ok_runs_block_with_value(void **ts) {
  result_t(int, int) res = result_init_ok(111);

  int received_value = 0;

  result_scoped_with_ok(res, value, {
    received_value = value;
  });

  assert_int_equal(111, received_value);
}

static void test_result_scoped_with_ok_or_else_for_err_does_not_run_block(void **ts) {
  result_t(int, int)res = result_init_err(111);

  result_scoped_with_ok_or_else(res, value, ({
    (void) value;
    fail();
  }), ({
    // nada
  }));
}

static void test_result_scoped_with_ok_or_else_for_err_runs_else_block(void **ts) {
  result_t(int, int)res = result_init_err(111);

  result_scoped_with_ok_or_else(res, value, ({
    (void) value;
  }), ({
    return;
  }));

  fail();
}

static void test_result_scoped_with_ok_or_else_for_ok_runs_block_with_value(void **ts) {
  result_t(int, int) res = result_init_ok(111);

  int received_value = 0;

  result_scoped_with_ok_or_else(res, value, ({
    received_value = value;
  }), ({
    fail();
  }))

  assert_int_equal(111, received_value);
}

static void test_result_scoped_with_ok_or_else_for_ok_does_not_run_else_block(void **ts) {
  result_t(int, int) res = result_init_ok(111);

  result_scoped_with_ok_or_else(res, value, ({
    (void) value;
  }), ({
    fail();
  }));
}

static void test_result_scoped_with_err_for_ok_does_nothing(void **ts) {
  result_t(int, int) res = result_init_ok(111);

  result_scoped_with_err(res, value, {
    (void) value;
    fail();
  });
}

static void test_result_scoped_with_err_for_err_runs_block_with_value(void **ts) {
  result_t(int, int) res = result_init_err(111);

  int received_value = 0;

  result_scoped_with_err(res, value, {
    received_value = value;
  });

  assert_int_equal(111, received_value);
}

static void test_result_scoped_with_err_or_else_for_ok_does_not_run_block(void **ts) {
  result_t(int, int)res = result_init_ok(111);

  result_scoped_with_err_or_else(res, value, ({
    (void) value;
    fail();
  }), ({
    // nada
  }));
}

static void test_result_scoped_with_err_or_else_for_ok_runs_else_block(void **ts) {
  result_t(int, int)res = result_init_ok(111);

  result_scoped_with_err_or_else(res, value, ({
    (void) value;
  }), ({
    return;
  }));

  fail();
}

static void test_result_scoped_with_err_or_else_for_err_runs_block_with_value(void **ts) {
  result_t(int, int) res = result_init_err(111);

  int received_value = 0;

  result_scoped_with_err_or_else(res, value, ({
    received_value = value;
  }), ({
    fail();
  }))

  assert_int_equal(111, received_value);
}

static void test_result_scoped_with_err_or_else_for_err_does_not_run_else_block(void **ts) {
  result_t(int, int) res = result_init_err(111);

  result_scoped_with_err_or_else(res, value, ({
    (void) value;
  }), ({
    fail();
  }));
}

#pragma GCC diagnostic pop

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_inits_ok_directly),
    cmocka_unit_test(test_inits_err_directly),
    cmocka_unit_test(test_updates_from_value_to_value),
    cmocka_unit_test(test_updates_from_err_to_err),
    cmocka_unit_test(test_updates_from_value_to_err),
    cmocka_unit_test(test_updates_from_err_to_value),
    cmocka_unit_test(test_updates_from_value_to_value_via_set),
    cmocka_unit_test(test_updates_from_err_to_err_via_set),
    cmocka_unit_test(test_updates_from_value_to_err_via_set),
    cmocka_unit_test(test_updates_from_err_to_value_via_set),
    cmocka_unit_test(test_and_returns_the_second_result_when_first_is_ok),
    cmocka_unit_test(test_and_returns_the_first_result_when_its_an_error),
    cmocka_unit_test(test_and_returns_the_first_result_when_both_are_errors),
    cmocka_unit_test(test_and_returns_the_second_result_when_both_are_ok),
    cmocka_unit_test(test_or_with_ok_and_err_returns_ok),
    cmocka_unit_test(test_or_with_err_and_ok_returns_ok),
    cmocka_unit_test(test_or_with_err_and_err_returns_the_second_err),
    cmocka_unit_test(test_or_with_ok_and_ok_returns_the_first_ok),
    cmocka_unit_test(test_unwrap_or_for_none_returns_default_value),
    cmocka_unit_test(test_unwrap_or_for_none_returns_original_value),
    cmocka_unit_test(test_result_with_ok_for_err_does_nothing),
    cmocka_unit_test(test_result_with_ok_for_err_runs_else_block),
    cmocka_unit_test(test_result_with_ok_for_ok_runs_block_with_value),
    cmocka_unit_test(test_result_with_ok_for_ok_does_not_run_else_block),
    cmocka_unit_test(test_result_with_err_for_ok_does_nothing),
    cmocka_unit_test(test_result_with_err_for_ok_runs_else_block),
    cmocka_unit_test(test_result_with_err_for_err_runs_block_with_value),
    cmocka_unit_test(test_result_with_err_for_err_does_not_run_else_block),
    cmocka_unit_test(test_unwrap_or_else_with_ok_returns_the_value),
    cmocka_unit_test(test_unwrap_or_else_with_err_returns_zeroed_value_and_runs_the_block),
    cmocka_unit_test(test_unwrap_or_else_with_ok_and_missing_else_block_returns_the_value),
    cmocka_unit_test(test_unwrap_or_else_with_err_and_missing_else_block_returns_zeroed_value),
    cmocka_unit_test(test_pads_to_multiple_of_pointer_size),
    cmocka_unit_test(test_allows_forward_declaration),
    cmocka_unit_test(test_result_scoped_with_ok_for_err_does_nothing),
    cmocka_unit_test(test_result_scoped_with_ok_for_ok_runs_block_with_value),
    cmocka_unit_test(test_result_scoped_with_ok_or_else_for_err_does_not_run_block),
    cmocka_unit_test(test_result_scoped_with_ok_or_else_for_err_runs_else_block),
    cmocka_unit_test(test_result_scoped_with_ok_or_else_for_ok_runs_block_with_value),
    cmocka_unit_test(test_result_scoped_with_ok_or_else_for_ok_does_not_run_else_block),
    cmocka_unit_test(test_result_scoped_with_err_for_ok_does_nothing),
    cmocka_unit_test(test_result_scoped_with_err_for_err_runs_block_with_value),
    cmocka_unit_test(test_result_scoped_with_err_or_else_for_ok_does_not_run_block),
    cmocka_unit_test(test_result_scoped_with_err_or_else_for_ok_runs_else_block),
    cmocka_unit_test(test_result_scoped_with_err_or_else_for_err_runs_block_with_value),
    cmocka_unit_test(test_result_scoped_with_err_or_else_for_err_does_not_run_else_block),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
