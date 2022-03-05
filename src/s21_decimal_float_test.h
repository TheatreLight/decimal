#ifndef SRC_S21_DECIMAL_FLOAT_TEST_H_
#define SRC_S21_DECIMAL_FLOAT_TEST_H_

#include <check.h>
#include <math.h>
#include <string.h>

#include "s21_decimal.h"

struct from_float_to_decimal_loop {
    float act_float;
    char *expected;
    int exp_conversion;
    value_type_t value_type;
    int is_null;
} float_to_decimal_td[] = {
    {15625, "15625", SUCCESS, s21_NORMAL_VALUE, 0},
    {0, "0", CONVERTING_ERROR, s21_NORMAL_VALUE, 1},
    {0, "0", CONVERTING_ERROR, s21_INFINITY, 0},
    {2.2e-30F, "", CONVERTING_ERROR, s21_NORMAL_VALUE, 0},
    // {2.2e-27F, "0.0000000000000000000000000022", SUCCESS, s21_NORMAL_VALUE,
    // 0},
    {2.2e30F, "", CONVERTING_ERROR, s21_NORMAL_VALUE, 0},
};

START_TEST(from_float_to_decimal_loop) {
    s21_decimal actual;
    s21_decimal *p_to_actual = &actual;
    if (float_to_decimal_td[_i].value_type == s21_NAN) {
        float_to_decimal_td[_i].act_float = NAN;
    } else if (float_to_decimal_td[_i].value_type == s21_INFINITY ||
               float_to_decimal_td[_i].value_type == s21_NEGATIVE_INFINITY) {
        float_to_decimal_td[_i].act_float = INFINITY;
    } else if (float_to_decimal_td[_i].is_null) {
        p_to_actual = NULL;
    }
    int conv_act = s21_from_float_to_decimal(float_to_decimal_td[_i].act_float,
                                             p_to_actual);
    ck_assert_int_eq(conv_act, float_to_decimal_td[_i].exp_conversion);
    if (conv_act == SUCCESS) {
        char actual_string[40];
        s21_from_decimal_to_string(*p_to_actual, actual_string);
        ck_assert_str_eq(actual_string, float_to_decimal_td[_i].expected);
    }
}
END_TEST

struct from_decimal_to_float_loop {
    char *flt;
    int exp_conversion;
    value_type_t value_type;
    int is_null;
} decimal_to_float_td[] = {
    {"15625", SUCCESS, s21_NORMAL_VALUE, 0},
    {"0", CONVERTING_ERROR, s21_NORMAL_VALUE, 1},
    {"0", CONVERTING_ERROR, s21_INFINITY, 0},
};

START_TEST(from_decimal_to_float_loop) {
    float actual;
    float *p_to_actual = &actual;
    if (float_to_decimal_td[_i].is_null) {
        p_to_actual = NULL;
    }
    s21_decimal flt;
    s21_from_string_to_decimal(decimal_to_float_td[_i].flt, &flt);
    flt.value_type = decimal_to_float_td[_i].value_type;
    int conv_act = s21_from_decimal_to_float(flt, p_to_actual);
    ck_assert_int_eq(conv_act, decimal_to_float_td[_i].exp_conversion);
    if (conv_act == SUCCESS) {
        char actual_string[40];
        s21_from_decimal_to_string(flt, actual_string);
        ck_assert_str_eq(actual_string, decimal_to_float_td[_i].flt);
    }
}
END_TEST

#endif  // SRC_S21_DECIMAL_FLOAT_TEST_H_
