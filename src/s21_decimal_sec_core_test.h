#ifndef SRC_S21_DECIMAL_SEC_CORE_TEST_H_
#define SRC_S21_DECIMAL_SEC_CORE_TEST_H_

#include <check.h>

#include "s21_decimal.h"

s21_decimal get_decimal(int i, float f, int is_int) {
  s21_decimal test_data;
  s21_init_decimal(&test_data);
  if (is_int) {
    s21_from_int_to_decimal(i, &test_data);
  } else {
    s21_from_float_to_decimal(f, &test_data);
  }
  return test_data;
}

START_TEST(get_float_exp) {
  float src = 0.15625;
  int act = -3;
  int exp = s21_get_float_exp(src);
  ck_assert_int_eq(act, exp);
}
END_TEST

struct compare_td {
  int i_left;
  float f_left;
  value_type_t value_type_left;
  int i_right;
  float f_right;
  value_type_t value_type_right;
  int is_int;
  int expected;
} compare_td[] = {
    {0, 0, s21_NEGATIVE_INFINITY, 0, 0, s21_NORMAL_VALUE, 1, -1},
    {0, 0, s21_NORMAL_VALUE, 0, 0, s21_INFINITY, 1, -1},
    {0, 0, s21_NEGATIVE_INFINITY, 0, 0, s21_INFINITY, 1, -1},
    {0, 0, s21_INFINITY, 0, 0, s21_NORMAL_VALUE, 1, 1},
    {0, 0, s21_NORMAL_VALUE, 0, 0, s21_NEGATIVE_INFINITY, 1, 1},
    {0, 0, s21_INFINITY, 0, 0, s21_NEGATIVE_INFINITY, 1, 1},
    {0, 0, s21_INFINITY, 0, 0, s21_INFINITY, 1, 0},
    {0, 0, s21_NEGATIVE_INFINITY, 0, 0, s21_NEGATIVE_INFINITY, 1, 0}};

START_TEST(compare_test) {
  s21_decimal left = get_decimal(compare_td[_i].i_left, compare_td[_i].f_left,
                                 compare_td[_i].is_int);
  s21_decimal right = get_decimal(
      compare_td[_i].i_right, compare_td[_i].f_right, compare_td[_i].is_int);
  left.value_type = compare_td[_i].value_type_left;
  right.value_type = compare_td[_i].value_type_right;
  int actual = s21_compare(left, right);
  ck_assert_int_eq(actual, compare_td[_i].expected);
}
END_TEST

struct is_can_normalized_td {
  int i;
  float f;
  int value_type;
  int scale;
  int is_int;
  int expected;
} is_can_normalized_td[] = {{9, 0, s21_NORMAL_VALUE, 0, 1, FALSE},
                            {10, 0, s21_NORMAL_VALUE, 0, 1, TRUE},
                            {10, 0, s21_NORMAL_VALUE, 1, 1, TRUE},
                            {156, 0, s21_NORMAL_VALUE, 1, 1, FALSE}};

START_TEST(is_can_normalized) {
  s21_decimal decimal =
      get_decimal(is_can_normalized_td[_i].i, is_can_normalized_td[_i].f,
                  is_can_normalized_td[_i].is_int);
  int act = s21_is_can_normalized(decimal);
  ck_assert_int_eq(act, is_can_normalized_td[_i].expected);
}
END_TEST

START_TEST(down_scale) {
  s21_decimal decimal =
      get_decimal(is_can_normalized_td[_i].i, is_can_normalized_td[_i].f,
                  is_can_normalized_td[_i].is_int);
  s21_set_scale(&decimal, is_can_normalized_td[_i].scale);
  s21_decimal exp =
      get_decimal(is_can_normalized_td[_i].i, is_can_normalized_td[_i].f,
                  is_can_normalized_td[_i].is_int);
  s21_set_scale(&exp, is_can_normalized_td[_i].scale);
  for (int i = LOW; s21_get_scale(exp) && i < SCALE; i++) {
    if (exp.bits[i] != 0) {
      exp.bits[i] /= 10;
      s21_set_scale(&exp, is_can_normalized_td[_i].scale - 1);
      break;
    }
  }
  s21_decimal act = s21_down_scale(decimal);
  ck_assert_int_eq(act.bits[LOW], exp.bits[LOW]);
  ck_assert_int_eq(act.bits[MID], exp.bits[MID]);
  ck_assert_int_eq(act.bits[HIGH], exp.bits[HIGH]);
  ck_assert_int_eq(act.bits[SCALE], exp.bits[SCALE]);
}
END_TEST

struct get_high_bit_td {
  int i;
  float f;
  int value_type;
  int is_int;
  int expected;
} get_high_bit_td[] = {{2147483647, 0, s21_NORMAL_VALUE, 1, 30},
                       {10, 0, s21_NORMAL_VALUE, 1, 3}};

START_TEST(get_high_bit) {
  s21_decimal decimal = get_decimal(
      get_high_bit_td[_i].i, get_high_bit_td[_i].f, get_high_bit_td[_i].is_int);
  int act = s21_get_high_bit(decimal);
  ck_assert_int_eq(act, get_high_bit_td[_i].expected);
}
END_TEST

struct div_bits_td {
  int i_left;
  float f_left;
  value_type_t value_type_left;
  int i_right;
  float f_right;
  value_type_t value_type_right;
  int is_int;
  int i_exp;
  float f_exp;
  value_type_t value_type_exp;
  int expected_scale;
} div_td[] = {
    {9000, 0, s21_NORMAL_VALUE, 3, 0, s21_NORMAL_VALUE, 1, 3000, 0,
     s21_NORMAL_VALUE, 0},
    {256000, 0, s21_NORMAL_VALUE, 128, 0, s21_NORMAL_VALUE, 1, 2000, 0,
     s21_NORMAL_VALUE, 0},
};

START_TEST(div_bits) {
  s21_decimal result;
  s21_init_decimal(&result);
  s21_decimal left =
      get_decimal(div_td[_i].i_left, div_td[_i].f_left, div_td[_i].is_int);
  s21_decimal right =
      get_decimal(div_td[_i].i_right, div_td[_i].f_right, div_td[_i].is_int);
  left.value_type = div_td[_i].value_type_left;
  right.value_type = div_td[_i].value_type_right;
  s21_div_bits(left, right, &result);
  ck_assert_int_eq(result.bits[0], div_td[_i].i_exp);
  ck_assert_int_eq(s21_get_scale(result), div_td[_i].expected_scale);
}
END_TEST

struct decimal_norm_td {
  int i;
  float f;
  int scale;
  int is_int;
  int expected_bit0;
  int expected_scale;
} decimal_norm_td[] = {{1000000000, 0, 10, 1, 1, 1}, {10, 0, 9, 1, 1, 8}};

START_TEST(decimal_norm) {
  s21_decimal decimal = get_decimal(
      decimal_norm_td[_i].i, decimal_norm_td[_i].f, decimal_norm_td[_i].is_int);
  s21_set_scale(&decimal, decimal_norm_td[_i].scale);
  s21_decimal_norm(&decimal);
  ck_assert_int_eq(decimal.bits[0], decimal_norm_td[_i].expected_bit0);
  ck_assert_int_eq(s21_get_scale(decimal), decimal_norm_td[_i].expected_scale);
}
END_TEST

struct is_can_div_td {
  int i_left;
  float f_left;
  value_type_t value_type_left;
  int i_right;
  float f_right;
  value_type_t value_type_right;
  int is_int;
  int expected;
  int expected_vt;
} is_can_div_td[] = {
    {1, 0, s21_NORMAL_VALUE, 1, 0, s21_NORMAL_VALUE, 1, TRUE, s21_NORMAL_VALUE},
    {0, 0, s21_NORMAL_VALUE, 0, 0, s21_NORMAL_VALUE, 1, FALSE, s21_NAN},
    {1, 0, s21_NORMAL_VALUE, 0, 0, s21_NORMAL_VALUE, 1, FALSE, s21_INFINITY},
    {-1, 0, s21_NORMAL_VALUE, 0, 0, s21_NORMAL_VALUE, 1, FALSE,
     s21_NEGATIVE_INFINITY},
    {1, 0, s21_INFINITY, 0, 0, s21_NORMAL_VALUE, 1, FALSE, s21_NAN},
    {1, 0, s21_NEGATIVE_INFINITY, 0, 0, s21_NORMAL_VALUE, 1, FALSE, s21_NAN},
    {1, 0, s21_INFINITY, 1, 0, s21_NEGATIVE_INFINITY, 1, FALSE, s21_NAN},
    {1, 0, s21_NEGATIVE_INFINITY, 1, 0, s21_INFINITY, 1, FALSE, s21_NAN},
    {1, 0, s21_INFINITY, 1, 0, s21_INFINITY, 1, FALSE, s21_NAN},
    {1, 0, s21_NEGATIVE_INFINITY, 1, 0, s21_NEGATIVE_INFINITY, 1, FALSE,
     s21_NAN},
    {1, 0, s21_NAN, 1, 0, s21_NORMAL_VALUE, 1, FALSE, s21_NAN},
    {1, 0, s21_NORMAL_VALUE, 1, 0, s21_NAN, 1, FALSE, s21_NAN},
};

START_TEST(is_can_div) {
  s21_decimal result;
  s21_init_decimal(&result);
  s21_decimal left =
      get_decimal(is_can_div_td[_i].i_left, is_can_div_td[_i].f_left,
                  is_can_div_td[_i].is_int);
  s21_decimal right =
      get_decimal(is_can_div_td[_i].i_right, is_can_div_td[_i].f_right,
                  is_can_div_td[_i].is_int);
  left.value_type = is_can_div_td[_i].value_type_left;
  right.value_type = is_can_div_td[_i].value_type_right;
  int actual = s21_is_can_div(left, right, &result);
  // ck_assert_msg();
  ck_assert_int_eq(actual, is_can_div_td[_i].expected);
  ck_assert_int_eq(result.value_type, is_can_div_td[_i].expected_vt);
}
END_TEST

#endif  // SRC_S21_DECIMAL_SEC_CORE_TEST_H_
