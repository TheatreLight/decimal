#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_
#include <limits.h>
#include <stdio.h>
#include <string.h>

#define MAX_BIT_CHAR 0b10000000
#define MIN_BIT 1
#define MAX_BIT 2147483648u
#define DECIMAL_DEFAULT \
  { {0, 0, 0, 0}, 0 }

typedef enum {
  s21_NORMAL_VALUE = 0,
  s21_INFINITY = 1,
  s21_NEGATIVE_INFINITY = 2,
  s21_NAN = 3
} value_type_t;

typedef struct Decimal {
  unsigned int bits[4];
  value_type_t value_type;
} s21_decimal;

typedef union float_to_int {
  unsigned int i;
  float f;
} float_to_int;


enum { LOW, MID, HIGH, SCALE };
enum { TRUE, FALSE, SUCCESS = 0, CONVERTING_ERROR };

s21_decimal s21_add(s21_decimal x, s21_decimal y);
s21_decimal s21_sub(s21_decimal x, s21_decimal y);
s21_decimal s21_mul(s21_decimal x, s21_decimal y);
s21_decimal s21_div(s21_decimal x, s21_decimal y);
s21_decimal s21_mod(s21_decimal x, s21_decimal y);

int s21_is_less(s21_decimal x, s21_decimal y);
int s21_is_less_or_equal(s21_decimal x, s21_decimal y);
int s21_is_greater(s21_decimal x, s21_decimal y);
int s21_is_greater_or_equal(s21_decimal x, s21_decimal y);
int s21_is_equal(s21_decimal x, s21_decimal y);
int s21_is_not_equal(s21_decimal x, s21_decimal y);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

s21_decimal s21_floor(s21_decimal x);
s21_decimal s21_round(s21_decimal x);
s21_decimal s21_truncate(s21_decimal x);
s21_decimal s21_negate(s21_decimal x);

s21_decimal s21_alg_add(s21_decimal x, s21_decimal y);
int s21_is_positive(s21_decimal x);
s21_decimal s21_invert(s21_decimal x);
s21_decimal s21_abs(s21_decimal x);
s21_decimal *s21_shift_right(s21_decimal *x);
s21_decimal *s21_shift_left(s21_decimal *x);
void s21_shift(s21_decimal *arg2, int n);
int s21_scale_align(s21_decimal *x, s21_decimal *y);

int s21_get_scale(s21_decimal dec);
void s21_set_scale(s21_decimal *dec, int scale);

int s21_get_sign(s21_decimal dec);
void s21_set_sign(s21_decimal *dec);
void s21_clear_sign(s21_decimal *dec);

void s21_set_bit(s21_decimal *dec, unsigned int index);
int s21_get_bit(s21_decimal dec, unsigned int index);

int s21_get_float_exp(float src);
void s21_init_decimal(s21_decimal *src);
int s21_get_high_bit(s21_decimal src);
int s21_is_can_normalized(s21_decimal src);
s21_decimal s21_down_scale(s21_decimal src);
s21_decimal s21_up_scale(s21_decimal src);
void s21_decimal_norm(s21_decimal *src);
int s21_compare(s21_decimal left, s21_decimal right);
s21_decimal s21_div_bits(s21_decimal dividend, s21_decimal divisor,
                         s21_decimal *result);
int s21_get_decimal_count(s21_decimal decimal);
int s21_is_can_div(s21_decimal x, s21_decimal y, s21_decimal *result);
s21_decimal s21_division_by_10(s21_decimal x);
int s21_rem_mod_10(s21_decimal x);
int s21_from_string_to_decimal(char *value, s21_decimal *result);
int s21_is_zero(s21_decimal decimal);
int s21_from_decimal_to_string(s21_decimal decimal, char *result);

s21_decimal dec_div_by_ten(s21_decimal src);
s21_decimal s21_small_sub(s21_decimal x, s21_decimal y);
s21_decimal s21_small_mul(s21_decimal x);

s21_decimal *s21_big_shift_left(s21_decimal *x);
#endif  // SRC_S21_DECIMAL_H_
