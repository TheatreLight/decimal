#ifndef SRC_S21_DECIMAL_MOD_TEST_H_
#define SRC_S21_DECIMAL_MOD_TEST_H_

#include <check.h>

#include "s21_decimal.h"

struct mod_td {
    char *dividend;
    char *devisor;
    char *expected;
    int conversion;
} mod_1td[] = {
    {"1", "3", "1", SUCCESS},
    // {"2", "-0.45", "0.2", SUCCESS},
    {"6521", "74121", "6521", SUCCESS},
    {"1", "987456321", "1", SUCCESS},
    {"-9878798789", "-3", "-2", SUCCESS},
    {"7922816251427554395", "65645646", "20894583", SUCCESS},
};

START_TEST(mod) {
    int conversion = SUCCESS;
    s21_decimal dividend, devisor;
    conversion = s21_from_string_to_decimal(mod_1td[_i].dividend, &dividend);
    ck_assert_int_eq(conversion, mod_1td[_i].conversion);
    conversion = s21_from_string_to_decimal(mod_1td[_i].devisor, &devisor);
    ck_assert_int_eq(conversion, mod_1td[_i].conversion);
    char act[40];
    s21_from_decimal_to_string(s21_mod(dividend, devisor), act);
    ck_assert_str_eq(mod_1td[_i].expected, act);
}
END_TEST

#endif  // SRC_S21_DECIMAL_MOD_TEST_H_
