#ifndef SRC_S21_DECIMAL_DIV_TEST_H_
#define SRC_S21_DECIMAL_DIV_TEST_H_

#include <check.h>

#include "s21_decimal.h"

struct div_td {
  char *left;
  char *right;
  char *expected;
  int conversion;
} div_1td[] = {
    {"325", "5", "65", SUCCESS},
    {"1", "3", "0.3333333333333333333333333333", SUCCESS},
    {"2", "3", "0.6666666666666666666666666667", SUCCESS},
    {"2", "-0.45", "-4.4444444444444444444444444444", SUCCESS},
    {"6521", "74121", "0.087977766085185035280150025", SUCCESS},
    {"1", "987456321", "0.0000000010127030216256016048", SUCCESS},
    {"-9878798789", "-3", "3292932929.6666666666666666667", SUCCESS},
    {"7922816251427554395", "65645646", "120690658622.31829350875761051",
     SUCCESS},
    {"2.2e10", "1e10", "2.2", SUCCESS},
};

START_TEST(div) {
  int conversion = SUCCESS;
  s21_decimal left, right, expected;
  conversion = s21_from_string_to_decimal(div_1td[_i].left, &left);
  ck_assert_int_eq(conversion, div_1td[_i].conversion);
  conversion = s21_from_string_to_decimal(div_1td[_i].right, &right);
  ck_assert_int_eq(conversion, div_1td[_i].conversion);
  conversion = s21_from_string_to_decimal(div_1td[_i].expected, &expected);
  ck_assert_int_eq(conversion, div_1td[_i].conversion);
  char act[40];
  s21_from_decimal_to_string(s21_div(left, right), act);
  ck_assert_msg(s21_is_equal(expected, s21_div(left, right)) == 0,
                "Assertion \'%s != %s\' Failed", div_1td[_i].expected, act);
}
END_TEST

#endif  // SRC_S21_DECIMAL_DIV_TEST_H_
