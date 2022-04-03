#include <check.h>

#include "s21_decimal.h"
#include "s21_decimal_div_test.h"
#include "s21_decimal_float_test.h"
#include "s21_decimal_mod_test.h"
#include "s21_decimal_sec_core_test.h"

START_TEST(s21_add_test1) {
    s21_decimal x;
    s21_decimal y;
    s21_decimal actual_res;
    s21_decimal expected_result;
    s21_init_decimal(&x);
    s21_init_decimal(&y);
    s21_init_decimal(&actual_res);
    s21_init_decimal(&expected_result);
    // simple values
    x.bits[LOW] = 3;  // 11
    y.bits[LOW] = 2;  // 10
    expected_result.bits[LOW] = 5;
    actual_res = s21_add(x, y);
    ck_assert_int_eq(actual_res.bits[LOW], expected_result.bits[LOW]);
    ck_assert_int_eq(actual_res.bits[MID], expected_result.bits[MID]);
    ck_assert_int_eq(actual_res.bits[HIGH], expected_result.bits[HIGH]);
}
END_TEST

START_TEST(s21_add_test2) {
    s21_decimal x;
    s21_decimal y;
    s21_decimal actual_result;
    s21_decimal expected_result;
    s21_init_decimal(&x);
    s21_init_decimal(&y);
    s21_init_decimal(&actual_result);
    s21_init_decimal(&expected_result);
    // simple values
    x.bits[LOW] =
        2147483647;  // 2147483647 --> 1111111111111111111111111111111 (31)
    y.bits[LOW] =
        2147483647;  // 2147483647 --> 1111111111111111111111111111111 (31)
    expected_result.bits[LOW] = 0b11111111111111111111111111111110;
    expected_result.bits[MID] = 0x00000000;
    expected_result.bits[HIGH] = 0x00000000;
    actual_result = s21_add(x, y);
    ck_assert_int_eq(actual_result.bits[LOW], expected_result.bits[LOW]);
    ck_assert_int_eq(actual_result.bits[MID], expected_result.bits[MID]);
    ck_assert_int_eq(actual_result.bits[HIGH], expected_result.bits[HIGH]);
}
END_TEST

START_TEST(s21_add_test3) {
    s21_decimal x;
    s21_decimal y;
    s21_decimal actual_result;
    s21_decimal expected_result;
    s21_init_decimal(&x);
    s21_init_decimal(&y);
    s21_init_decimal(&actual_result);
    s21_init_decimal(&expected_result);
    x.bits[LOW] = 4294967295;
    y.bits[LOW] = 4294967295;
    x.bits[SCALE] = 0x80000000;
    y.bits[SCALE] = 0x80000000;
    expected_result.bits[LOW] = 0b11111111111111111111111111111110;
    expected_result.bits[MID] = 0x00000001;
    expected_result.bits[HIGH] = 0x00000000;
    expected_result.bits[SCALE] = 0x80000000;  // 32 разряд "-"

    actual_result = s21_add(x, y);
    ck_assert_int_eq(actual_result.bits[LOW], expected_result.bits[LOW]);
    ck_assert_int_eq(actual_result.bits[MID], expected_result.bits[MID]);
    ck_assert_int_eq(actual_result.bits[HIGH], expected_result.bits[HIGH]);
}
END_TEST

START_TEST(s21_add_test4) {
    s21_decimal x;
    s21_decimal y;
    s21_decimal actual_result;
    s21_decimal expected_result;
    s21_init_decimal(&x);
    s21_init_decimal(&y);
    s21_init_decimal(&actual_result);
    s21_init_decimal(&expected_result);
    x.bits[LOW] = 4294967295U;
    x.bits[SCALE] = 0x80000000;
    y.bits[LOW] = 2147483647;
    expected_result.bits[LOW] = 2147483648;
    expected_result.bits[MID] = 0x00000000;
    expected_result.bits[HIGH] = 0;
    expected_result.bits[SCALE] = 0x80000000U;
    actual_result = s21_add(x, y);
    ck_assert_int_eq(actual_result.bits[LOW], expected_result.bits[LOW]);
    ck_assert_int_eq(actual_result.bits[MID], expected_result.bits[MID]);
    ck_assert_int_eq(actual_result.bits[HIGH], expected_result.bits[HIGH]);
    ck_assert_int_eq(actual_result.bits[SCALE], expected_result.bits[SCALE]);
}
END_TEST

// неактуальный тест (требуется настроить обработку s21_infinity )
START_TEST(s21_add_test5) {
    s21_decimal x;
    s21_decimal y;
    s21_decimal actual_result;
    s21_decimal expected_result;
    s21_init_decimal(&x);
    s21_init_decimal(&y);
    s21_init_decimal(&actual_result);
    s21_init_decimal(&expected_result);
    x.bits[LOW] = 5;
    x.value_type = s21_INFINITY;
    y.bits[LOW] = 3;
    y.value_type = s21_INFINITY;
    expected_result.bits[LOW] = 8;
    expected_result.bits[MID] = 0;
    expected_result.bits[HIGH] = 0;
    actual_result = s21_add(x, y);
    ck_assert_int_eq(actual_result.bits[LOW], expected_result.bits[LOW]);
    ck_assert_int_eq(actual_result.bits[MID], expected_result.bits[MID]);
    ck_assert_int_eq(actual_result.bits[HIGH], expected_result.bits[HIGH]);
    ck_assert_int_eq(actual_result.bits[SCALE], expected_result.bits[SCALE]);
}
END_TEST

START_TEST(add0) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);
    s21_decimal x;
    x.bits[0] = 0b00000000000000000000000000000000;
    x.bits[1] = 0b00000000000000000000000000000000;
    x.bits[2] = 0b00000000000000000000000000000000;
    x.bits[3] = 0b10000000000000010000000000000000;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 0b00000000000000000000000000000011;
    y.bits[1] = 0b00000000000000000000000000000000;
    y.bits[2] = 0b00000000000000000000000000000000;
    y.bits[3] = 0b00000000000000000000000000000000;
    y.value_type = s21_NORMAL_VALUE;

    s21_decimal z;
    z.bits[0] = 0b00000000000000000000000000011110;
    z.bits[1] = 0b00000000000000000000000000000000;
    z.bits[2] = 0b00000000000000000000000000000000;
    z.bits[3] = 0b00000000000000010000000000000000;

    decimal = s21_add(x, y);
    ck_assert_int_eq(decimal.bits[0], z.bits[0]);
    ck_assert_int_eq(decimal.bits[1], z.bits[1]);
    ck_assert_int_eq(decimal.bits[2], z.bits[2]);
    ck_assert_int_eq(decimal.bits[3], z.bits[3]);
}
END_TEST

START_TEST(add) {  // 3229647104 + 1283490051 = 4513137155
    s21_decimal decimal;
    s21_init_decimal(&decimal);
    s21_decimal x;
    x.bits[0] = 3229647104u;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 1283490051;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    s21_decimal z;
    z.bits[0] = 218169859;
    z.bits[1] = 1;
    z.bits[2] = 0;
    z.bits[3] = 0;

    decimal = s21_add(x, y);
    ck_assert_int_eq(decimal.bits[0], z.bits[0]);
    ck_assert_int_eq(decimal.bits[1], z.bits[1]);
    ck_assert_int_eq(decimal.bits[2], z.bits[2]);
    ck_assert_int_eq(decimal.bits[3], z.bits[3]);
}
END_TEST

START_TEST(
    add1) {  // -4294967298000000000 + -4294968579490051 = -4,299262267×10¹⁸
    s21_decimal decimal;
    s21_init_decimal(&decimal);
    s21_decimal x;
    x.bits[0] = 2000000000;
    x.bits[1] = 1000000000;
    x.bits[2] = 0;
    x.bits[3] = MAX_BIT;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 1283490051;
    y.bits[1] = 1000000;
    y.bits[2] = 0;
    y.bits[3] = MAX_BIT;
    y.value_type = s21_NORMAL_VALUE;

    s21_decimal z;
    z.bits[0] = 3283490051u;
    z.bits[1] = 1001000000;
    z.bits[2] = 0;
    z.bits[3] = MAX_BIT;

    decimal = s21_add(x, y);
    ck_assert_int_eq(decimal.bits[0], z.bits[0]);
    ck_assert_int_eq(decimal.bits[1], z.bits[1]);
    ck_assert_int_eq(decimal.bits[2], z.bits[2]);
    ck_assert_int_eq(decimal.bits[3], z.bits[3]);
}
END_TEST

START_TEST(
    add2) {  // 4,299262267×10¹⁸ + -4294968579490051 = 4294967298000000000
    s21_decimal decimal;
    s21_init_decimal(&decimal);
    s21_decimal x;
    x.bits[0] = 3283490051u;
    x.bits[1] = 1001000000;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 1283490051;
    y.bits[1] = 1000000;
    y.bits[2] = 0;
    y.bits[3] = MAX_BIT;
    y.value_type = s21_NORMAL_VALUE;

    s21_decimal z;
    z.bits[0] = 2000000000;
    z.bits[1] = 1000000000;
    z.bits[2] = 0;
    z.bits[3] = 0;

    decimal = s21_add(x, y);
    ck_assert_int_eq(decimal.bits[0], z.bits[0]);
    ck_assert_int_eq(decimal.bits[1], z.bits[1]);
    ck_assert_int_eq(decimal.bits[2], z.bits[2]);
    ck_assert_int_eq(decimal.bits[3], z.bits[3]);
}
END_TEST

START_TEST(add3) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);
    s21_decimal x;
    x.bits[0] = 2805989376u;
    x.bits[1] = 3358472169u;
    x.bits[2] = 975;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 2805989376u;
    y.bits[1] = 3358472169u;
    y.bits[2] = 975;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    s21_decimal z;
    z.bits[0] = 1317011456;
    z.bits[1] = 2421977043u;
    z.bits[2] = 1951;
    z.bits[3] = 0;

    decimal = s21_add(x, y);
    ck_assert_int_eq(decimal.bits[0], z.bits[0]);
    ck_assert_int_eq(decimal.bits[1], z.bits[1]);
    ck_assert_int_eq(decimal.bits[2], z.bits[2]);
    ck_assert_int_eq(decimal.bits[3], z.bits[3]);
}
END_TEST

START_TEST(s21_sub_test1) {
    s21_decimal x;
    s21_decimal y;
    s21_decimal actual_result;
    s21_decimal expected_result;
    s21_init_decimal(&x);
    s21_init_decimal(&y);
    s21_init_decimal(&actual_result);
    s21_init_decimal(&expected_result);
    x.bits[LOW] = UINT32_MAX;
    s21_set_sign(&x);
    y.bits[LOW] = 2147483647;
    expected_result.bits[LOW] = 0b01111111111111111111111111111110;
    expected_result.bits[MID] = 1;
    expected_result.bits[HIGH] = 0;
    expected_result.bits[SCALE] = 0x80000000;
    actual_result = s21_sub(x, y);
    ck_assert_int_eq(actual_result.bits[LOW], expected_result.bits[LOW]);
    ck_assert_int_eq(actual_result.bits[MID], expected_result.bits[MID]);
    ck_assert_int_eq(actual_result.bits[HIGH], expected_result.bits[HIGH]);
    ck_assert_int_eq(actual_result.bits[SCALE], expected_result.bits[SCALE]);
}
END_TEST

START_TEST(s21_sub_test2) {
    s21_decimal x;
    s21_decimal y;
    s21_decimal actual_result;
    s21_decimal expected_result;
    s21_init_decimal(&x);
    s21_init_decimal(&y);
    s21_init_decimal(&actual_result);
    s21_init_decimal(&expected_result);
    x.bits[LOW] = 0xFFFFFFFF;
    x.bits[MID] = 0xFFFFFFFF;
    x.bits[HIGH] = 0;
    s21_set_sign(&y);
    y.bits[LOW] = 0xFFFFFFFF;
    expected_result.bits[LOW] = 0xFFFFFFFE;
    expected_result.bits[MID] = 0;
    expected_result.bits[HIGH] = 1;
    actual_result = s21_sub(x, y);
    ck_assert_int_eq(actual_result.bits[LOW], expected_result.bits[LOW]);
    ck_assert_int_eq(actual_result.bits[MID], expected_result.bits[MID]);
    ck_assert_int_eq(actual_result.bits[HIGH], expected_result.bits[HIGH]);
    ck_assert_int_eq(actual_result.bits[SCALE], expected_result.bits[SCALE]);
}
END_TEST

START_TEST(s21_sub_test3) {
    s21_decimal x;
    s21_decimal y;
    s21_decimal actual_result;
    s21_decimal expected_result;
    s21_init_decimal(&x);
    s21_init_decimal(&y);
    s21_init_decimal(&actual_result);
    s21_init_decimal(&expected_result);
    x.bits[LOW] = 0xFFFFFFFF;
    x.bits[MID] = 0x0000FFFF;
    x.bits[HIGH] = 0;
    s21_set_sign(&y);
    s21_set_sign(&x);
    y.bits[LOW] = 0xFFFFFFFF;
    expected_result.bits[LOW] = 0;
    expected_result.bits[MID] = 0b00000000000000001111111111111111;
    expected_result.bits[HIGH] = 0;
    expected_result.bits[SCALE] = 0x80000000;
    actual_result = s21_sub(x, y);
    ck_assert_int_eq(actual_result.bits[LOW], expected_result.bits[LOW]);
    ck_assert_int_eq(actual_result.bits[MID], expected_result.bits[MID]);
    ck_assert_int_eq(actual_result.bits[HIGH], expected_result.bits[HIGH]);
    ck_assert_int_eq(actual_result.bits[SCALE], expected_result.bits[SCALE]);
}
END_TEST

START_TEST(mul1) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);
    s21_decimal x;
    x.bits[0] = 2000000000;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 2000000000;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    s21_decimal z;
    z.bits[0] = 2643460096u;
    z.bits[1] = 931322574;
    z.bits[2] = 0;
    z.bits[3] = 0;

    decimal = s21_mul(x, y);
    ck_assert_int_eq(decimal.bits[0], z.bits[0]);
    ck_assert_int_eq(decimal.bits[1], z.bits[1]);
    ck_assert_int_eq(decimal.bits[2], z.bits[2]);
    ck_assert_int_eq(decimal.bits[3], z.bits[3]);
}
END_TEST

START_TEST(mul2_n) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);
    s21_decimal x;
    x.bits[0] = 3000000000u;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = MAX_BIT;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 2000000000;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    s21_decimal z;
    z.bits[0] = 3965190144u;
    z.bits[1] = 1396983861;
    z.bits[2] = 0;
    z.bits[3] = MAX_BIT;

    decimal = s21_mul(x, y);
    ck_assert_int_eq(decimal.bits[0], z.bits[0]);
    ck_assert_int_eq(decimal.bits[1], z.bits[1]);
    ck_assert_int_eq(decimal.bits[2], z.bits[2]);
    ck_assert_int_eq(decimal.bits[3], z.bits[3]);
}
END_TEST

START_TEST(mul2_n_1) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);
    s21_decimal x;
    x.bits[0] = 4000000000u;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 2150000000u;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = MAX_BIT;
    y.value_type = s21_NORMAL_VALUE;

    s21_decimal z;
    z.bits[0] = 1817968640;
    z.bits[1] = 2002343535;
    z.bits[2] = 0;
    z.bits[3] = MAX_BIT;

    decimal = s21_mul(x, y);
    ck_assert_int_eq(decimal.bits[0], z.bits[0]);
    ck_assert_int_eq(decimal.bits[1], z.bits[1]);
    ck_assert_int_eq(decimal.bits[2], z.bits[2]);
    ck_assert_int_eq(decimal.bits[3], z.bits[3]);
}
END_TEST

START_TEST(mul3_n) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);
    s21_decimal x;
    x.bits[0] = 3000000000u;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = MAX_BIT;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 3000000000u;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = MAX_BIT;
    y.value_type = s21_NORMAL_VALUE;

    s21_decimal z;
    z.bits[0] = 3800301568u;
    z.bits[1] = 2095475792;
    z.bits[2] = 0;
    z.bits[3] = 0;

    decimal = s21_mul(x, y);
    ck_assert_int_eq(decimal.bits[0], z.bits[0]);
    ck_assert_int_eq(decimal.bits[1], z.bits[1]);
    ck_assert_int_eq(decimal.bits[2], z.bits[2]);
    ck_assert_int_eq(decimal.bits[3], z.bits[3]);
}
END_TEST

START_TEST(mul4) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);
    s21_decimal x;
    x.bits[0] = 4000000000u;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 4000000000u;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    s21_decimal z;
    z.bits[0] = 1983905792;
    z.bits[1] = 3725290298;
    z.bits[2] = 0;
    z.bits[3] = 0;

    decimal = s21_mul(x, y);
    ck_assert_int_eq(decimal.bits[0], z.bits[0]);
    ck_assert_int_eq(decimal.bits[1], z.bits[1]);
    ck_assert_int_eq(decimal.bits[2], z.bits[2]);
    ck_assert_int_eq(decimal.bits[3], z.bits[3]);
}
END_TEST

START_TEST(mul5) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);
    s21_decimal x;
    x.bits[0] = 1215752192;
    x.bits[1] = 23;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 1215752192;
    y.bits[1] = 23;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    s21_decimal z;
    z.bits[0] = 2990538752u;
    z.bits[1] = 434162106;
    z.bits[2] = 542;
    z.bits[3] = 0;

    decimal = s21_mul(x, y);
    ck_assert_int_eq(decimal.bits[0], z.bits[0]);
    ck_assert_int_eq(decimal.bits[1], z.bits[1]);
    ck_assert_int_eq(decimal.bits[2], z.bits[2]);
    ck_assert_int_eq(decimal.bits[3], z.bits[3]);
}
END_TEST

START_TEST(mul) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    s21_decimal z;
    z.bits[0] = 1489240064;
    z.bits[1] = 1525798795;
    z.bits[2] = 1;
    z.bits[3] = 0;

    decimal = s21_mul(x, y);
    ck_assert_int_eq(decimal.bits[0], z.bits[0]);
    ck_assert_int_eq(decimal.bits[1], z.bits[1]);
    ck_assert_int_eq(decimal.bits[2], z.bits[2]);
    ck_assert_int_eq(decimal.bits[3], z.bits[3]);
}
END_TEST

START_TEST(is_great1) {  // x > y
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 1;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    int res = TRUE;
    int decimal = s21_is_greater(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_great2) {  // x > y
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = MAX_BIT;
    y.value_type = s21_NORMAL_VALUE;

    int res = TRUE;
    int decimal = s21_is_greater(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_great3) {  // x > y
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = MAX_BIT;
    y.value_type = s21_NORMAL_VALUE;

    int res = TRUE;
    int decimal = s21_is_greater(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_great4) {  // x == y
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 0;
    x.bits[3] = MAX_BIT;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = MAX_BIT;
    y.value_type = s21_NORMAL_VALUE;

    int res = FALSE;
    int decimal = s21_is_greater(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_great_or_eq1) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    int res = TRUE;
    int decimal = s21_is_greater_or_equal(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_great_or_eq2) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    int res = TRUE;
    int decimal = s21_is_greater_or_equal(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_great_or_eq3) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    int res = FALSE;
    int decimal = s21_is_greater_or_equal(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_less1) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 1;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    int res = FALSE;
    int decimal = s21_is_less(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_less2) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    int res = TRUE;
    int decimal = s21_is_less(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_less3) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = MAX_BIT;
    y.value_type = s21_NORMAL_VALUE;

    int res = FALSE;
    int decimal = s21_is_less(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_less4) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 0;
    x.bits[3] = MAX_BIT;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = MAX_BIT;
    y.value_type = s21_NORMAL_VALUE;

    int res = FALSE;
    int decimal = s21_is_less(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_less_or_eq1) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    int res = FALSE;
    int decimal = s21_is_less_or_equal(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_less_or_eq2) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    int res = TRUE;
    int decimal = s21_is_less_or_equal(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_less_or_eq3) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    int res = TRUE;
    int decimal = s21_is_less_or_equal(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_equal1) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 1;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    int res = FALSE;
    int decimal = s21_is_equal(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_equal2) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    int res = FALSE;
    int decimal = s21_is_equal(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_equal3) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = MAX_BIT;
    y.value_type = s21_NORMAL_VALUE;

    int res = FALSE;
    int decimal = s21_is_equal(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_equal4) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 0;
    x.bits[3] = MAX_BIT;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = MAX_BIT;
    y.value_type = s21_NORMAL_VALUE;

    int res = TRUE;
    int decimal = s21_is_equal(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_not_equal1) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 1;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    int res = TRUE;
    int decimal = s21_is_not_equal(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_not_equal2) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;

    int res = TRUE;
    int decimal = s21_is_not_equal(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_not_equal3) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = MAX_BIT;
    y.value_type = s21_NORMAL_VALUE;

    int res = TRUE;
    int decimal = s21_is_not_equal(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(is_not_equal4) {
    s21_decimal x;
    x.bits[0] = 705032704;
    x.bits[1] = 1;
    x.bits[2] = 0;
    x.bits[3] = MAX_BIT;
    x.value_type = s21_NORMAL_VALUE;

    s21_decimal y;
    y.bits[0] = 705032704;
    y.bits[1] = 1;
    y.bits[2] = 0;
    y.bits[3] = MAX_BIT;
    y.value_type = s21_NORMAL_VALUE;

    int res = FALSE;
    int decimal = s21_is_not_equal(x, y);
    ck_assert_int_eq(decimal, res);
}
END_TEST

START_TEST(truncate) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);

    s21_decimal x;
    x.bits[0] = 123456789;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;
    s21_set_scale(&x, 4);

    s21_decimal y;
    y.bits[0] = 12345;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;

    decimal = s21_truncate(x);
    ck_assert_int_eq(decimal.bits[0], y.bits[0]);
    ck_assert_int_eq(decimal.bits[1], y.bits[1]);
    ck_assert_int_eq(decimal.bits[2], y.bits[2]);
    ck_assert_int_eq(decimal.bits[3], y.bits[3]);
}
END_TEST

START_TEST(truncate1) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);

    s21_decimal x;
    x.bits[0] = 2345678987;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;
    s21_set_scale(&x, 4);

    s21_decimal y;
    y.bits[0] = 234567;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;

    decimal = s21_truncate(x);
    ck_assert_int_eq(decimal.bits[0], y.bits[0]);
    ck_assert_int_eq(decimal.bits[1], y.bits[1]);
    ck_assert_int_eq(decimal.bits[2], y.bits[2]);
    ck_assert_int_eq(decimal.bits[3], y.bits[3]);
}
END_TEST

START_TEST(truncate2) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);

    s21_decimal x;
    x.bits[0] = 2345678987;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;
    s21_set_scale(&x, 21);

    s21_decimal y;
    y.bits[0] = 0;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;

    decimal = s21_truncate(x);
    ck_assert_int_eq(decimal.bits[0], y.bits[0]);
    ck_assert_int_eq(decimal.bits[1], y.bits[1]);
    ck_assert_int_eq(decimal.bits[2], y.bits[2]);
    ck_assert_int_eq(decimal.bits[3], y.bits[3]);
}
END_TEST

START_TEST(truncate3) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);

    s21_decimal x;
    x.bits[0] = 2345678987;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;
    s21_set_scale(&x, 4);
    s21_set_sign(&x);

    s21_decimal y;
    y.bits[0] = 234567;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;
    s21_set_sign(&y);

    decimal = s21_truncate(x);
    ck_assert_int_eq(decimal.bits[0], y.bits[0]);
    ck_assert_int_eq(decimal.bits[1], y.bits[1]);
    ck_assert_int_eq(decimal.bits[2], y.bits[2]);
    ck_assert_int_eq(decimal.bits[3], y.bits[3]);
}
END_TEST

START_TEST(truncate4) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);

    s21_decimal x;
    x.bits[0] = 2345678987;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NAN;
    s21_set_scale(&x, 2);

    s21_decimal y;
    y.bits[0] = 0;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;

    decimal = s21_truncate(x);
    ck_assert_int_eq(decimal.bits[0], y.bits[0]);
    ck_assert_int_eq(decimal.bits[1], y.bits[1]);
    ck_assert_int_eq(decimal.bits[2], y.bits[2]);
    ck_assert_int_eq(decimal.bits[3], y.bits[3]);
}
END_TEST

START_TEST(decfloor) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);

    s21_decimal x;
    x.bits[0] = 123456789;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;
    s21_set_scale(&x, 4);

    s21_decimal y;
    y.bits[0] = 12345;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;

    decimal = s21_floor(x);
    ck_assert_int_eq(decimal.bits[0], y.bits[0]);
    ck_assert_int_eq(decimal.bits[1], y.bits[1]);
    ck_assert_int_eq(decimal.bits[2], y.bits[2]);
    ck_assert_int_eq(decimal.bits[3], y.bits[3]);
}
END_TEST

START_TEST(decfloor1) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);

    s21_decimal x;
    x.bits[0] = 123456789;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;
    s21_set_scale(&x, 4);
    s21_set_sign(&x);

    s21_decimal y;
    y.bits[0] = 12346;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;
    s21_set_sign(&y);

    decimal = s21_floor(x);
    ck_assert_int_eq(decimal.bits[0], y.bits[0]);
    ck_assert_int_eq(decimal.bits[1], y.bits[1]);
    ck_assert_int_eq(decimal.bits[2], y.bits[2]);
    ck_assert_int_eq(decimal.bits[3], y.bits[3]);
}
END_TEST

START_TEST(decfloor2) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);

    s21_decimal x;
    x.bits[0] = 123456789;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_INFINITY;
    s21_set_scale(&x, 4);

    s21_decimal y;
    y.bits[0] = 0;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;

    decimal = s21_floor(x);
    ck_assert_int_eq(decimal.bits[0], y.bits[0]);
    ck_assert_int_eq(decimal.bits[1], y.bits[1]);
    ck_assert_int_eq(decimal.bits[2], y.bits[2]);
    ck_assert_int_eq(decimal.bits[3], y.bits[3]);
}
END_TEST

START_TEST(decfloor3) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);

    s21_decimal x;
    x.bits[0] = 0;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;
    s21_set_scale(&x, 4);
    s21_set_sign(&x);

    s21_decimal y;
    y.bits[0] = 0;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;
    s21_set_sign(&y);

    decimal = s21_floor(x);
    ck_assert_int_eq(decimal.bits[0], y.bits[0]);
    ck_assert_int_eq(decimal.bits[1], y.bits[1]);
    ck_assert_int_eq(decimal.bits[2], y.bits[2]);
    ck_assert_int_eq(decimal.bits[3], y.bits[3]);
}
END_TEST

START_TEST(negate) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);

    s21_decimal x;
    x.bits[0] = 123456789;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;
    x = s21_negate(x);

    s21_decimal y;
    y.bits[0] = 123456789;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;
    s21_set_sign(&y);

    decimal = x;
    ck_assert_int_eq(decimal.bits[0], y.bits[0]);
    ck_assert_int_eq(decimal.bits[1], y.bits[1]);
    ck_assert_int_eq(decimal.bits[2], y.bits[2]);
    ck_assert_int_eq(decimal.bits[3], y.bits[3]);
}
END_TEST

START_TEST(negate1) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);

    s21_decimal x;
    x.bits[0] = 123456789;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;
    s21_set_sign(&x);
    x = s21_negate(x);

    s21_decimal y;
    y.bits[0] = 123456789;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;

    decimal = x;
    ck_assert_int_eq(decimal.bits[0], y.bits[0]);
    ck_assert_int_eq(decimal.bits[1], y.bits[1]);
    ck_assert_int_eq(decimal.bits[2], y.bits[2]);
    ck_assert_int_eq(decimal.bits[3], y.bits[3]);
}
END_TEST

START_TEST(negate2) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);

    s21_decimal x;
    x.bits[0] = 123456789;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NAN;
    s21_set_sign(&x);
    x = s21_negate(x);

    s21_decimal y;
    y.bits[0] = 0;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;

    decimal = x;
    ck_assert_int_eq(decimal.bits[0], y.bits[0]);
    ck_assert_int_eq(decimal.bits[1], y.bits[1]);
    ck_assert_int_eq(decimal.bits[2], y.bits[2]);
    ck_assert_int_eq(decimal.bits[3], y.bits[3]);
}
END_TEST

START_TEST(decround) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);

    s21_decimal x;
    x.bits[0] = 23455555;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;
    s21_set_scale(&x, 4);
    s21_set_sign(&x);

    s21_decimal y;
    y.bits[0] = 2346;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;
    s21_set_sign(&y);

    decimal = s21_round(x);
    ck_assert_int_eq(decimal.bits[0], y.bits[0]);
    ck_assert_int_eq(decimal.bits[1], y.bits[1]);
    ck_assert_int_eq(decimal.bits[2], y.bits[2]);
    ck_assert_int_eq(decimal.bits[3], y.bits[3]);
}
END_TEST

START_TEST(decround1) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);

    s21_decimal x;
    x.bits[0] = 23455555;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;
    s21_set_scale(&x, 4);

    s21_decimal y;
    y.bits[0] = 2346;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;

    decimal = s21_round(x);
    ck_assert_int_eq(decimal.bits[0], y.bits[0]);
    ck_assert_int_eq(decimal.bits[1], y.bits[1]);
    ck_assert_int_eq(decimal.bits[2], y.bits[2]);
    ck_assert_int_eq(decimal.bits[3], y.bits[3]);
}
END_TEST

START_TEST(decround2) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);

    s21_decimal x;
    x.bits[0] = 23455555;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NAN;

    s21_decimal y;
    y.bits[0] = 0;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;

    decimal = s21_round(x);
    ck_assert_int_eq(decimal.bits[0], y.bits[0]);
    ck_assert_int_eq(decimal.bits[1], y.bits[1]);
    ck_assert_int_eq(decimal.bits[2], y.bits[2]);
    ck_assert_int_eq(decimal.bits[3], y.bits[3]);
}
END_TEST

START_TEST(decround3) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);

    s21_decimal x;
    x.bits[0] = 234;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;
    s21_set_scale(&x, 1);
    s21_set_sign(&x);

    s21_decimal y;
    y.bits[0] = 23;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;
    y.value_type = s21_NORMAL_VALUE;
    s21_set_sign(&y);

    decimal = s21_round(x);
    ck_assert_int_eq(decimal.bits[0], y.bits[0]);
    ck_assert_int_eq(decimal.bits[1], y.bits[1]);
    ck_assert_int_eq(decimal.bits[2], y.bits[2]);
    ck_assert_int_eq(decimal.bits[3], y.bits[3]);
}
END_TEST

START_TEST(decround4) {
    s21_decimal decimal;
    s21_init_decimal(&decimal);

    s21_decimal x;
    x.bits[0] = 2344;
    x.bits[1] = 0;
    x.bits[2] = 0;
    x.bits[3] = 0;
    x.value_type = s21_NORMAL_VALUE;
    s21_set_scale(&x, 3);

    s21_decimal y;
    y.bits[0] = 2;
    y.bits[1] = 0;
    y.bits[2] = 0;
    y.bits[3] = 0;

    decimal = s21_round(x);
    ck_assert_int_eq(decimal.bits[0], y.bits[0]);
    ck_assert_int_eq(decimal.bits[1], y.bits[1]);
    ck_assert_int_eq(decimal.bits[2], y.bits[2]);
    ck_assert_int_eq(decimal.bits[3], y.bits[3]);
}
END_TEST

// s21_from_decimal_to_int_test1  with SCALE = 2
START_TEST(s21_from_decimal_to_int_test1) {
    s21_decimal x;
    s21_init_decimal(&x);
    x.bits[LOW] = 483647;
    s21_set_scale(&x, 2);
    int expected_result = 4836;
    int actual_result = 0;
    s21_from_decimal_to_int(x, &actual_result);
    ck_assert_int_eq(actual_result, expected_result);
}
END_TEST

// s21_from_decimal_to_int_test2  without SCALE
START_TEST(s21_from_decimal_to_int_test2) {
    s21_decimal x;
    s21_init_decimal(&x);
    x.bits[LOW] = 483647;
    int expected_result = 483647;
    int actual_result = 0;
    s21_from_decimal_to_int(x, &actual_result);
    ck_assert_int_eq(actual_result, expected_result);
}
END_TEST

// s21_from_decimal_to_int_test2  with SIGN = 1
START_TEST(s21_from_decimal_to_int_test3) {
    s21_decimal x;
    s21_init_decimal(&x);
    x.bits[LOW] = 483647;
    s21_set_sign(&x);
    int expected_result = -483647;
    int actual_result = 0;
    s21_from_decimal_to_int(x, &actual_result);
    ck_assert_int_eq(actual_result, expected_result);
}
END_TEST

// s21_from_decimal_to_int_test2  with s21_INFINITY = 1,
START_TEST(s21_from_decimal_to_int_test4) {
    s21_decimal x;
    s21_init_decimal(&x);
    x.bits[LOW] = 483647;
    x.value_type = s21_INFINITY;
    int expected_result = CONVERTING_ERROR;
    int actual_result = 0, result = 0;
    if (s21_from_decimal_to_int(x, &actual_result)) {
        result = CONVERTING_ERROR;
    }

    ck_assert_int_eq(result, expected_result);
}
END_TEST

// s21_from_decimal_to_int_test2  with
START_TEST(s21_from_decimal_to_int_test5) {
    s21_decimal x;
    s21_init_decimal(&x);
    x.bits[LOW] = 483647;
    x.bits[MID] = 1;
    x.bits[HIGH] = 1;
    int expected_result = CONVERTING_ERROR;
    int actual_result = s21_from_decimal_to_int(x, &actual_result);
    ck_assert_int_eq(actual_result, expected_result);
}
END_TEST

START_TEST(s21_from_decimal_to_float_test1) {
    s21_decimal src;
    float dst = 0.0f;
    float expected_result = 0.15F;
    s21_init_decimal(&src);
    s21_set_scale(&src, 2);
    src.bits[LOW] = 15;
    src.bits[MID] = 0;
    src.bits[HIGH] = 0;
    s21_from_decimal_to_float(src, &dst);
    ck_assert_msg((dst == expected_result), NULL);
}
END_TEST

START_TEST(s21_from_decimal_to_float_test2) {
    s21_decimal src;
    float *p_dst = NULL;
    float *p_expected_result = NULL;
    s21_init_decimal(&src);
    s21_set_scale(&src, 2);
    src.bits[LOW] = 15;
    src.bits[MID] = 0;
    src.bits[HIGH] = 0;
    s21_from_decimal_to_float(src, p_dst);
    ck_assert_ptr_eq(p_dst, p_expected_result);
}
END_TEST

START_TEST(s21_from_decimal_to_float_test3) {
    s21_decimal src;
    float dst = 0.0f;
    float expected_result = -0.15F;
    s21_init_decimal(&src);
    s21_set_sign(&src);
    s21_set_scale(&src, 2);
    src.bits[LOW] = 15;
    src.bits[MID] = 0;
    src.bits[HIGH] = 0;
    s21_from_decimal_to_float(src, &dst);
    ck_assert_msg((dst == expected_result), NULL);
}
END_TEST


int main(void) {
    Suite *s = suite_create("Core");
    TCase *tcc = tcase_create("secondary core");
    SRunner *sr = srunner_create(s);
    int test_result = 0;

    suite_add_tcase(s, tcc);
    tcase_add_test(tcc, get_float_exp);
    tcase_add_loop_test(tcc, compare_test, 0,
                        sizeof(compare_td) / sizeof(compare_td[0]));
    tcase_add_loop_test(
        tcc, is_can_normalized, 0,
        sizeof(is_can_normalized_td) / sizeof(is_can_normalized_td[0]));
    tcase_add_loop_test(tcc, get_high_bit, 0,
                        sizeof(get_high_bit_td) / sizeof(get_high_bit_td[0]));
    tcase_add_loop_test(tcc, decimal_norm, 0,
                        sizeof(decimal_norm_td) / sizeof(decimal_norm_td[0]));
    tcase_add_loop_test(tcc, is_can_div, 0,
                        sizeof(is_can_div_td) / sizeof(is_can_div_td[0]));

    TCase *tcd = tcase_create("decimal core");
    suite_add_tcase(s, tcd);
    tcase_add_loop_test(
        tcd, from_float_to_decimal_loop, 0,
        sizeof(float_to_decimal_td) / sizeof(float_to_decimal_td[0]));
    tcase_add_loop_test(
        tcd, from_decimal_to_float_loop, 0,
        sizeof(decimal_to_float_td) / sizeof(decimal_to_float_td[0]));
    tcase_add_loop_test(tcd, div, 0, sizeof(div_1td) / sizeof(div_1td[0]));
    tcase_add_loop_test(tcd, mod, 0, sizeof(mod_1td) / sizeof(mod_1td[0]));
    tcase_add_loop_test(tcc, div_bits, 0, sizeof(div_td) / sizeof(div_td[0]));
    tcase_add_loop_test(
        tcc, down_scale, 2,
        sizeof(is_can_normalized_td) / sizeof(is_can_normalized_td[0]));
    tcase_add_test(tcd, add);
    tcase_add_test(tcd, add0);
    tcase_add_test(tcd, add1);
    tcase_add_test(tcd, add2);
    tcase_add_test(tcd, add3);
    tcase_add_test(tcd, s21_add_test1);
    tcase_add_test(tcd, s21_add_test2);
    tcase_add_test(tcd, s21_add_test3);
    tcase_add_test(tcd, s21_add_test4);
    tcase_add_test(tcd, s21_add_test5);
    tcase_add_test(tcd, s21_sub_test1);
    tcase_add_test(tcd, s21_sub_test2);
    tcase_add_test(tcd, s21_sub_test3);

    tcase_add_test(tcd, mul1);
    tcase_add_test(tcd, mul2_n);
    tcase_add_test(tcd, mul2_n_1);
    tcase_add_test(tcd, mul3_n);
    tcase_add_test(tcd, mul4);
    tcase_add_test(tcd, mul5);
    tcase_add_test(tcd, mul);
    tcase_add_test(tcd, is_great1);
    tcase_add_test(tcd, is_great2);
    tcase_add_test(tcd, is_great3);
    tcase_add_test(tcd, is_great4);
    tcase_add_test(tcd, is_great_or_eq1);
    tcase_add_test(tcd, is_great_or_eq2);
    tcase_add_test(tcd, is_great_or_eq3);
    tcase_add_test(tcd, is_less1);
    tcase_add_test(tcd, is_less2);
    tcase_add_test(tcd, is_less3);
    tcase_add_test(tcd, is_less4);
    tcase_add_test(tcd, is_less_or_eq1);
    tcase_add_test(tcd, is_less_or_eq2);
    tcase_add_test(tcd, is_less_or_eq3);
    tcase_add_test(tcd, is_equal1);
    tcase_add_test(tcd, is_equal2);
    tcase_add_test(tcd, is_equal3);
    tcase_add_test(tcd, is_equal4);
    tcase_add_test(tcd, is_not_equal1);
    tcase_add_test(tcd, is_not_equal2);
    tcase_add_test(tcd, is_not_equal3);
    tcase_add_test(tcd, is_not_equal4);
    tcase_add_test(tcd, s21_from_decimal_to_int_test1);
    tcase_add_test(tcd, s21_from_decimal_to_int_test2);
    tcase_add_test(tcd, s21_from_decimal_to_int_test3);
    tcase_add_test(tcd, s21_from_decimal_to_int_test4);
    tcase_add_test(tcd, s21_from_decimal_to_int_test5);
    tcase_add_test(tcd, s21_from_decimal_to_float_test1);
    tcase_add_test(tcd, s21_from_decimal_to_float_test2);
    tcase_add_test(tcd, s21_from_decimal_to_float_test3);
    tcase_add_test(tcd, compare_test);

    tcase_add_test(tcd, truncate);
    tcase_add_test(tcd, truncate1);
    tcase_add_test(tcd, truncate2);
    tcase_add_test(tcd, truncate3);
    tcase_add_test(tcd, truncate4);
    tcase_add_test(tcd, decfloor);
    tcase_add_test(tcd, decfloor1);
    tcase_add_test(tcd, decfloor2);
    tcase_add_test(tcd, decfloor3);
    tcase_add_test(tcd, negate);
    tcase_add_test(tcd, negate1);
    tcase_add_test(tcd, negate2);
    tcase_add_test(tcd, decround);
    tcase_add_test(tcd, decround1);
    tcase_add_test(tcd, decround2);
    tcase_add_test(tcd, decround3);
    tcase_add_test(tcd, decround4);

    srunner_run_all(sr, CK_VERBOSE);
    test_result = srunner_ntests_failed(sr);
    srunner_free(sr);

    return test_result == 0 ? 0 : 1;
}
