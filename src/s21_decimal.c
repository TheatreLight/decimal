#include "s21_decimal.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* 1 сложение */
s21_decimal s21_add(s21_decimal x, s21_decimal y) {
    s21_decimal res;
    int scale = 0;
    int is_greater = s21_is_greater(s21_abs(x), s21_abs(y));
    if (s21_is_positive(x) & s21_is_positive(y)) {
        scale = s21_scale_align(&x, &y);
        res = s21_alg_add(x, y);
        s21_set_scale(&res, scale);
    } else if ((!s21_is_positive(x)) & s21_is_positive(y)) {
        scale = s21_scale_align(&x, &y);
        x = s21_invert(x);
        res = s21_alg_add(y, x);
        s21_set_scale(&res, scale);
        if (is_greater == TRUE) {
            res = s21_invert(res);
            s21_set_scale(&res, scale);
            s21_set_sign(&res);
        }
    } else if (s21_is_positive(x) & (!s21_is_positive(y))) {
        scale = s21_scale_align(&x, &y);
        y = s21_invert(y);
        res = s21_alg_add(x, y);
        s21_set_scale(&res, scale);
        if (is_greater == FALSE) {
            res = s21_invert(res);
            s21_set_scale(&res, scale);
            s21_set_sign(&res);
        }
    } else {
        scale = s21_scale_align(&x, &y);
        res = s21_alg_add(x, y);
        s21_set_sign(&res);
        s21_set_scale(&res, scale);
    }
    return res;
}

/* 2 вычитание */
s21_decimal s21_sub(s21_decimal x, s21_decimal y) {
    s21_decimal res;
    int scale = 0;
    int is_greater = s21_is_greater(s21_abs(x), s21_abs(y));
    if (s21_is_positive(x) & s21_is_positive(y)) {
        scale = s21_scale_align(&x, &y);
        y = s21_invert(y);
        res = s21_alg_add(x, y);
        s21_set_scale(&res, scale);
        if (is_greater == FALSE) {
            res = s21_invert(res);
            s21_set_scale(&res, scale);
            s21_set_sign(&res);
        }
    } else if ((!s21_is_positive(x)) & s21_is_positive(y)) {
        scale = s21_scale_align(&x, &y);
        res = s21_alg_add(x, y);
        s21_set_scale(&res, scale);
        s21_set_sign(&res);
    } else if (s21_is_positive(x) & (!s21_is_positive(y))) {
        scale = s21_scale_align(&x, &y);
        res = s21_alg_add(x, y);
        s21_set_scale(&res, scale);
    } else {
        scale = s21_scale_align(&x, &y);
        x = s21_invert(x);
        res = s21_alg_add(y, x);
        s21_set_scale(&res, scale);
        if (is_greater == TRUE) {
            res = s21_invert(res);
            s21_set_scale(&res, scale);
            s21_set_sign(&res);
        }
    }
    return res;
}

/* 3 умножение */
s21_decimal s21_mul(s21_decimal x, s21_decimal y) {
    s21_decimal res;
    s21_init_decimal(&res);
    int sign_x = s21_get_sign(x);
    int sign_y = s21_get_sign(y);
    int temp = y.bits[0] + y.bits[1] + y.bits[2];
    while (temp != 0) {
        int mask = y.bits[0] & MIN_BIT;
        if (mask == MIN_BIT) {
            res = s21_alg_add(res, x);
        }
        s21_shift_right(&y);
        s21_shift_left(&x);
        temp = y.bits[0] + y.bits[1] + y.bits[2];
    }
    s21_set_scale(&res, (s21_get_scale(x) + s21_get_scale(y)));
    if (sign_x && (!sign_y))
        s21_set_sign(&res);
    else if (sign_y && (!sign_x))
        s21_set_sign(&res);
    return res;
}

/* 4 деление */
s21_decimal s21_div(s21_decimal x, s21_decimal y) {
    s21_decimal result;
    s21_init_decimal(&result);
    if (s21_is_can_div(x, y, &result) == TRUE) {
        s21_decimal mod;
        s21_decimal_norm(&x);
        s21_decimal_norm(&y);
        int sign = ((s21_get_sign(x) + s21_get_sign(y)) % 2 == 0) ? 0 : 1;
        s21_clear_sign(&x);
        s21_clear_sign(&y);
        int scale = s21_scale_align(&x, &y);
        s21_set_scale(&x, scale);
        s21_set_scale(&y, scale);
        if (s21_is_can_div(x, y, &result) == TRUE) {
            s21_decimal temp;
            s21_init_decimal(&temp);
            do {
                // int dec_count = 0;
                int temp_scale = s21_get_scale(temp);
                s21_init_decimal(&temp);
                s21_set_scale(&temp, temp_scale);
                mod = s21_div_bits(x, y, &temp);
                result = s21_add(result, temp);
                s21_set_scale(&temp, s21_get_scale(result) + 1);
                mod = s21_up_scale(mod);
                x = mod;
                if (!s21_is_zero(mod) && s21_get_decimal_count(result) == 29) {
                    s21_decimal ten = {{10, 0, 0, 0}, s21_NORMAL_VALUE};
                    mod = s21_div_bits(result, ten, NULL);
                    if (s21_get_bit(mod, 0) % 2 != 0) {
                        s21_decimal one = {{1, 0, 0, 0}, s21_NORMAL_VALUE};
                        s21_set_scale(&one, s21_get_scale(result));
                        result = s21_add(result, one);
                    }
                }
            } while (!s21_is_zero(mod) &&
                     (s21_get_decimal_count(result) <= 28 &&
                      s21_get_scale(result) < 28));
            if (sign) s21_set_sign(&result);
        }
    }
    s21_decimal_norm(&result);
    return result;
}

// /* 5 остаток */
s21_decimal s21_mod(s21_decimal dividend, s21_decimal devisor) {
    s21_decimal mod;
    s21_init_decimal(&mod);
    if (s21_is_can_div(dividend, devisor, &mod) == TRUE) {
        int sign = s21_get_sign(dividend);
        s21_clear_sign(&dividend);
        s21_clear_sign(&devisor);
        s21_decimal quotient;
        s21_init_decimal(&quotient);
        int scale = s21_scale_align(&dividend, &devisor);
        s21_set_scale(&dividend, scale);
        s21_set_scale(&devisor, scale);
        mod = s21_div_bits(dividend, devisor, &quotient);
        if (sign) s21_set_sign(&mod);
        s21_decimal_norm(&mod);
    }
    return mod;
}

// /* 6 < */
int s21_is_less(s21_decimal x, s21_decimal y) {
    int result = FALSE;
    if (s21_compare(x, y) < 0) result = TRUE;
    return result;
}

// /* 7 <= */
int s21_is_less_or_equal(s21_decimal x, s21_decimal y) {
    int result = FALSE;
    if (s21_compare(x, y) <= 0) result = TRUE;
    return result;
}

/* 8 > */
int s21_is_greater(s21_decimal x, s21_decimal y) {
    int result = FALSE;
    if (s21_compare(x, y) > 0) result = TRUE;
    return result;
}

// /* 9 >= */
int s21_is_greater_or_equal(s21_decimal x, s21_decimal y) {
    int result = FALSE;
    if (s21_compare(x, y) >= 0) result = TRUE;
    return result;
}

// /* 10 == */
int s21_is_equal(s21_decimal x, s21_decimal y) {
    int result = FALSE;
    if (s21_compare(x, y) == 0) result = TRUE;
    return result;
}

// /* 11 != */
int s21_is_not_equal(s21_decimal x, s21_decimal y) {
    int result = FALSE;
    if (s21_compare(x, y) != 0) result = TRUE;
    return result;
}

/* 12 */
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    int conversion = SUCCESS;
    if (!dst) {
        conversion = CONVERTING_ERROR;
    }
    s21_init_decimal(dst);
    unsigned int shift = 1;
    if (src < 0) {
        src = abs(src);
        dst->bits[3] |= MAX_BIT;
    }
    int k = 32;
    while (k > 0) {
        unsigned int src_sh = (unsigned int)src & shift;
        if (src_sh == shift) {
            dst->bits[0] |= shift;
        }
        shift <<= 1;
        k--;
    }
    return conversion;
}

/* 13 */
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    int conversion = SUCCESS;
    if (!dst) {
        return CONVERTING_ERROR;
    }
    s21_init_decimal(dst);
    if (isnan(src) || isinf(src)) {
        conversion = CONVERTING_ERROR;
    } else if (src != 0) {
        int exp = s21_get_float_exp(src);
        if (exp < 96) {
            int scale = 0;
            double f = (double)src;
            if (exp > -95) {
                for (; !((int)f); f *= 10) {
                    scale++;
                }
            }
            if (exp > -94 && scale < 28) {
                if (src < 0) {
                    s21_set_sign(dst);
                    src = -src;
                }
                for (int i = 0; !(((int)f) / 10000000) && scale < 28;
                     f *= 10, i++) {
                    scale++;
                }
                src = (float)f;
                exp = s21_get_float_exp(src);
                s21_set_bit(dst, exp);
                int mask = 0x400000;
                float_to_int int_float;
                int_float.f = src;
                for (int i = exp - 1; i >= 0; i--, mask >>= 1) {
                    if (mask & int_float.i) {
                        s21_set_bit(dst, i);
                    }
                }
                s21_set_scale(dst, scale);
                s21_decimal_norm(dst);
            } else {
                conversion = CONVERTING_ERROR;
            }
        } else {
            conversion = CONVERTING_ERROR;
        }
    }
    return conversion;
}

// /* 14 */
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    // test1
    int result = 0;
    if (src.value_type == s21_NORMAL_VALUE && dst != NULL) {
        if ((src.bits[HIGH] != 0) || (src.bits[MID] != 0)) {
            result = CONVERTING_ERROR;
        } else {
            int scale = s21_get_scale(src) * (-1);
            if (scale != 0) {
                if ((src.bits[LOW] <=
                     (int)0x7FFFFFFF) /* || src.bits[LOW] == (int)(-1) */) {
                    *dst = (double)src.bits[LOW] * pow(10, scale);
                }
            } else {
                *dst = src.bits[LOW];
            }
            int sign = s21_get_sign(src);
            if (sign) {
                *dst = -(*dst);
            }
        }
    } else {
        result = CONVERTING_ERROR;
    }
    return result;
}

/* 15 */
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    int conversion = SUCCESS;
    if (!dst) {
        conversion = CONVERTING_ERROR;
    } else {
        if (src.value_type == s21_NORMAL_VALUE) {
            double f = 0;
            for (int i = 0; i < 96; i++) {
                if (s21_get_bit(src, i)) {
                    f += pow(2.0, i);
                }
            }
            int scale = s21_get_scale(src) * -1;
            f *= pow(10, scale);
            *dst = *dst * pow(10, scale);
            *dst = f;
            if (s21_get_sign(src)) {
                *dst = -*dst;
            }
        } else {
            conversion = CONVERTING_ERROR;
        }
    }
    return conversion;
}

/* 16 в меньшую сторону */
s21_decimal s21_floor(s21_decimal x) {
    s21_decimal res = x;
    if (res.value_type == s21_NORMAL_VALUE) {
        int sign = s21_get_sign(res);
        s21_clear_sign(&res);
        s21_decimal to_floor;
        s21_init_decimal(&to_floor);
        to_floor.bits[0] = 1;
        int flag = 0;
        int scale = s21_get_scale(x);
        if (scale) {
            for (int i = 0; i < scale; i++) {
                int anyrem = s21_rem_mod_10(x);
                x = s21_division_by_10(x);
                if (anyrem) {
                    flag = 1;
                }
            }
        }
        res = s21_truncate(res);
        if (sign) {
            if (flag) {
                res = s21_add(res, to_floor);
            }
            s21_set_sign(&res);
        }
    } else {
        s21_init_decimal(&res);
        res.value_type = x.value_type;
    }
    return res;
}

// /* 17 математическое округление */
s21_decimal s21_round(s21_decimal x) {
    s21_decimal res = x;
    if (res.value_type == s21_NORMAL_VALUE) {
        s21_decimal one;
        s21_init_decimal(&one);
        one.bits[0] = 1;
        int scale = s21_get_scale(res);
        int sign = s21_get_sign(res);
        res.bits[3] = 0;
        for (; scale > 0; scale--) {
            int rem = s21_rem_mod_10(res);
            if (rem >= 5) {
                res = s21_division_by_10(res);
                if (res.bits[0] != 0) {
                    res = s21_add(res, one);
                }
            } else {
                res = s21_division_by_10(res);
            }
        }
        if (sign) {
            s21_set_sign(&res);
        }
    } else {
        s21_init_decimal(&res);
        res.value_type = x.value_type;
    }
    return res;
}

/* 18 отброс дробной части */
s21_decimal s21_truncate(s21_decimal x) {
    s21_decimal res = x;
    if (x.value_type == s21_NORMAL_VALUE) {
        int scale = s21_get_scale(res);
        int sign = s21_get_sign(res);
        res.bits[3] = 0;
        for (; scale > 0; scale--) {
            res = s21_division_by_10(res);
        }
        if (sign) {
            s21_set_sign(&res);
        }
    } else {
        s21_init_decimal(&res);
        res.value_type = x.value_type;
    }
    return res;
}

/* 19 умножение на -1 */
s21_decimal s21_negate(s21_decimal x) {
    s21_decimal res = x;
    if (res.value_type == s21_NORMAL_VALUE) {
        int sign = s21_get_sign(res);
        if (sign) {
            s21_clear_sign(&res);
        } else {
            s21_set_sign(&res);
        }
    } else {
        s21_init_decimal(&res);
        res.value_type = x.value_type;
    }
    return res;
}

/* это побитовые сдвиги для массива интов */

s21_decimal *s21_shift_right(s21_decimal *x) {
    for (int i = 0; i < 3; i++) {
        if (i != 0) {
            unsigned int temp = x->bits[i] & MIN_BIT;
            x->bits[i] >>= MIN_BIT;
            if (temp == MIN_BIT) {
                x->bits[i - 1] |= MAX_BIT;
            }
        } else {
            x->bits[i] >>= MIN_BIT;
        }
    }
    return x;
}

s21_decimal *s21_shift_left(s21_decimal *x) {
    for (int i = 2; i >= 0; i--) {
        if (i != 2) {
            unsigned int temp = x->bits[i] & MAX_BIT;
            x->bits[i] <<= MIN_BIT;
            if (temp == MAX_BIT) {
                x->bits[i + 1] |= MIN_BIT;
            }
        } else {
            x->bits[i] <<= MIN_BIT;
        }
    }
    return x;
}

void s21_shift(s21_decimal *arg2, int n) {
    if (n > 0) {
        for (int i = n; i > 0; i--) {
            s21_shift_right(arg2);
        }
    } else {
        for (int i = n; i < 0; i++) {
            s21_shift_left(arg2);
        }
    }
}

/* алгоритм сложения */
s21_decimal s21_alg_add(s21_decimal x, s21_decimal y) {
    s21_decimal z;
    s21_init_decimal(&z);
    int temp = 0;
    for (int i = 0; i < 3; i++) {
        unsigned int res = 0;
        for (int k = 32, shift = 1; k > 0; k--, shift <<= 1) {
            int x_sh = x.bits[i] & shift;
            int y_sh = y.bits[i] & shift;
            if ((!x_sh) && y_sh && (!temp)) {
                res = res | shift;
            } else if (x_sh && (!y_sh) && (!temp)) {
                res = res | shift;
            } else if (x_sh && y_sh && (!temp)) {
                temp++;
            } else if ((!x_sh) && (!y_sh) && temp) {
                res = res | shift;
                temp = 0;
            } else if (x_sh && y_sh && temp) {
                res = res | shift;
            }
        }
        z.bits[i] = res;
    }
    return z;
}

/* проверка на положительный децимал */
int s21_is_positive(s21_decimal x) {
    int true = 0;
    if (!(x.bits[3] & MAX_BIT)) true = 1;
    return true;
}
/* перевод в доп код */
s21_decimal s21_invert(s21_decimal x) {
    for (int i = 0; i < 3; i++) {
        x.bits[i] = ~x.bits[i];
    }
    s21_decimal term;
    s21_init_decimal(&term);
    term.bits[0] = 1;
    return s21_alg_add(x, term);
}
/* модуль */
s21_decimal s21_abs(s21_decimal x) {
    s21_decimal ret = x;
    char *p_to_scale = (char *)&(ret.bits[SCALE]) + 3 * sizeof(char);
    unsigned int shift = MAX_BIT;
    if (ret.bits[3] & shift) *p_to_scale = 0;
    return ret;
}

/* выравнование скейлов */
int s21_scale_align(s21_decimal *x, s21_decimal *y) {
  int sign_x = s21_get_sign(*x);
  int sign_y = s21_get_sign(*y);
  int scale_x = s21_get_scale(*x);
  int scale_y = s21_get_scale(*y);
  int scale_res = scale_x;
  int flag = 0;
  s21_decimal temp;
  s21_init_decimal(&temp);
  if (scale_x > scale_y) {
    scale_res = scale_y;
    for (int i = 0; i < abs(scale_x - scale_y); i++) {
      temp = s21_small_mul(*y);
      if (temp.bits[3] != 0) {
        flag = 1;
        break;
      }
      *y = temp;
      scale_res++;
    }
  } else if (scale_x < scale_y) {
    scale_res = scale_x;
    for (int i = 0; i < abs(scale_x - scale_y); i++) {
      temp = s21_small_mul(*x);
      if (temp.bits[3] != 0) {
        flag = -1;
        break;
      }
      *x = temp;
      scale_res++;
    }
  }
  if (flag > 0) {
    for (int i = 0; i < abs(scale_res - scale_x); i++) {
      *x = dec_div_by_ten(*x);
    }
  } else if (flag < 0) {
    for (int i = 0; i < abs(scale_res - scale_y); i++) {
      *y = dec_div_by_ten(*y);
    }
  }
  s21_set_scale(x, scale_res);
  s21_set_scale(y, scale_res);
  if (sign_x) s21_set_sign(x);
  else if (sign_y) s21_set_sign(y);
  return scale_res;
}

// в bits[3] между  16 - 23 битами
int s21_get_scale(s21_decimal dec) {
    unsigned char *p_to_scale =
        (unsigned char *)&(dec.bits[SCALE]) + 2 * sizeof(char);
    return (int)*p_to_scale;
}

// в bits[3] между  16 - 23 битами
void s21_set_scale(s21_decimal *dec, int scale) {
    char *p_to_scale = (char *)&(dec->bits[SCALE]) + 2 * sizeof(char);
    *p_to_scale = (char)scale;
}

// получение знака
int s21_get_sign(s21_decimal dec) {
    unsigned int mask = MAX_BIT;
    return !!(mask & dec.bits[SCALE]);
}

// выставление знака
void s21_set_sign(s21_decimal *dec) {
    unsigned int mask = MAX_BIT;
    dec->bits[SCALE] |= mask;
}

void s21_clear_sign(s21_decimal *dec) {
    unsigned int mask = ~MAX_BIT;
    dec->bits[SCALE] &= mask;
}

// Функции для работы с отдельными битами:

int s21_get_bit(s21_decimal dec, unsigned int index) {
    unsigned int mask = 1u << (index % 32);
    return (index < 96) ? (!!(mask & dec.bits[index / 32])) : 0;
}

void s21_set_bit(s21_decimal *dec, unsigned int index) {
    unsigned int mask = 1u << (index % 32);
    dec->bits[index / 32] |= mask;
}

int s21_get_float_exp(float src) {
    int exp = 0;
    float_to_int int_float;
    int_float.f = src;
    unsigned int mask = 0x40000000;
    for (int i = 8; i > 0; mask >>= 1, i--) {
        if (int_float.i & mask) {
            exp += (int)pow(2, i - 1);
        }
    }
    return exp - 127;
}

void s21_init_decimal(s21_decimal *src) {
    src->bits[0] = 0;
    src->bits[1] = 0;
    src->bits[2] = 0;
    src->bits[3] = 0;
    src->value_type = s21_NORMAL_VALUE;
}

int s21_get_high_bit(s21_decimal src) {
    int high_bit = -1;
    if (!s21_is_zero(src) && src.value_type == s21_NORMAL_VALUE) {
        for (int i = 95; i >= 0; i--) {
            if (s21_get_bit(src, i) == 1) {
                high_bit = i;
                break;
            }
        }
    }
    return high_bit;
}

int s21_is_can_normalized(s21_decimal src) {
    int sum = 0;
    int sum_chet = 0;
    int sum_nchet = 0;
    for (int i = 0; i < 96; i++) {
        if ((i / 2) % 2 == 0) {
            if (i % 2 == 0 && s21_get_bit(src, i)) {
                sum_chet += 1;
            } else if (i % 2 == 1 && s21_get_bit(src, i)) {
                sum_chet += 2;
            }
        } else {
            if (i % 2 == 0 && s21_get_bit(src, i)) {
                sum_nchet += 1;
            } else if (i % 2 == 1 && s21_get_bit(src, i)) {
                sum_nchet += 2;
            }
        }
    }
    sum = sum_chet - sum_nchet;
    s21_decimal dec_sum;
    s21_init_decimal(&dec_sum);
    s21_from_int_to_decimal(sum, &dec_sum);
    return (sum % 5) == 0 ? s21_get_bit(src, 0) == 0 ? TRUE : FALSE : FALSE;
}

s21_decimal s21_div_bits(s21_decimal dividend, s21_decimal divisor,
                         s21_decimal *result) {
    s21_decimal zero;
    s21_init_decimal(&zero);
    if (!result) {
        result = &zero;
    }
    int scale = s21_get_scale(dividend);
    s21_set_scale(&dividend, 0);
    s21_set_scale(&divisor, 0);
    int max = s21_get_high_bit(divisor) - s21_get_high_bit(dividend);
    if (max <= 0) {
        s21_shift(&divisor, max);
        if (s21_is_less_or_equal(divisor, dividend) == TRUE) {
            s21_shift(&divisor, max * (-1));
        } else {
            s21_shift(&divisor, max * (-1));
            max = (max + 1);
        }
        for (int i = max; i <= 0; i++) {
            s21_decimal temp;
            s21_init_decimal(&temp);
            temp = s21_add(temp, divisor);
            s21_shift(&temp, i);
            if (s21_is_greater(temp, dividend) == TRUE) {
                s21_shift_left(result);
            } else {
                dividend = s21_sub(dividend, temp);
                s21_shift_left(result);
                s21_set_bit(result, 0);
            }
        }
    }
    s21_set_scale(&dividend, scale);
    return dividend;
}

int s21_is_can_div(s21_decimal x, s21_decimal y, s21_decimal *result) {
    int is_can_div = TRUE;
    if (x.value_type == s21_NAN || y.value_type == s21_NAN) {
        result->value_type = s21_NAN;
        is_can_div = FALSE;
    } else if ((x.value_type == s21_INFINITY ||
                x.value_type == s21_NEGATIVE_INFINITY) &&
               s21_is_zero(y)) {
        result->value_type = s21_NAN;
        is_can_div = FALSE;
    } else if (x.value_type == s21_NORMAL_VALUE &&
               y.value_type == s21_NORMAL_VALUE && s21_is_zero(x) &&
               s21_is_zero(y)) {
        result->value_type = s21_NAN;
        is_can_div = FALSE;
    } else if (x.value_type == s21_NORMAL_VALUE && s21_is_zero(y)) {
        if (s21_get_sign(x) == 0) {
            result->value_type = s21_INFINITY;
            is_can_div = FALSE;
        } else {
            result->value_type = s21_NEGATIVE_INFINITY;
            is_can_div = FALSE;
        }
    } else if ((x.value_type == s21_INFINITY ||
                x.value_type == s21_NEGATIVE_INFINITY) &&
               (y.value_type == s21_INFINITY ||
                y.value_type == s21_NEGATIVE_INFINITY)) {
        result->value_type = s21_NAN;
        is_can_div = FALSE;
    }
    return is_can_div;
}

int s21_get_decimal_count(s21_decimal decimal) {
    s21_decimal temp = decimal;
    int num_count = 0;
    while (!s21_is_zero(temp)) {
        num_count++;
        s21_decimal dividend, divisor, quotient;
        dividend = temp;
        s21_init_decimal(&divisor);
        s21_init_decimal(&quotient);
        s21_from_int_to_decimal(10, &divisor);
        s21_div_bits(dividend, divisor, &quotient);
        temp = quotient;
    }
    return num_count;
}

s21_decimal s21_down_scale(s21_decimal src) {
    s21_decimal div, result;
    s21_init_decimal(&div);
    s21_init_decimal(&result);
    div.bits[0] = 10u;
    int scale = s21_get_scale(src);
    s21_set_scale(&src, 0);
    int sign = s21_get_sign(src);
    s21_clear_sign(&src);
    s21_div_bits(src, div, &result);
    if (sign) s21_set_sign(&result);
    if (scale != 0) s21_set_scale(&result, scale - 1);
    return result;
}

s21_decimal s21_up_scale(s21_decimal src) {
    s21_decimal mul, result;
    s21_init_decimal(&mul);
    s21_init_decimal(&result);
    mul.bits[0] = 10u;
    int scale = s21_get_scale(src);
    s21_set_scale(&src, 0);
    int sign = s21_get_sign(src);
    s21_clear_sign(&src);
    result = s21_mul(src, mul);
    if (sign) s21_set_sign(&result);
    s21_set_scale(&result, scale + 1);
    return result;
}

void s21_decimal_norm(s21_decimal *src) {
    while (s21_is_can_normalized(*src) == TRUE && s21_get_scale(*src) > 0) {
        *src = s21_down_scale(*src);
    }
}

int s21_compare(s21_decimal left, s21_decimal right) {
    int result = 0;
    int leftsign = s21_get_sign(left);
    int rightsign = s21_get_sign(right);
    if ((left.value_type == s21_NEGATIVE_INFINITY &&
         right.value_type == s21_NORMAL_VALUE) ||
        (left.value_type == s21_NORMAL_VALUE &&
         right.value_type == s21_INFINITY) ||
        (left.value_type == s21_NEGATIVE_INFINITY &&
         right.value_type == s21_INFINITY)) {
        result = -1;
    } else if ((left.value_type == s21_INFINITY &&
                right.value_type == s21_NORMAL_VALUE) ||
               (left.value_type == s21_NORMAL_VALUE &&
                right.value_type == s21_NEGATIVE_INFINITY) ||
               (left.value_type == s21_INFINITY &&
                right.value_type == s21_NEGATIVE_INFINITY)) {
        result = 1;
    } else if ((left.value_type == s21_INFINITY &&
                right.value_type == s21_INFINITY) ||
               (left.value_type == s21_NEGATIVE_INFINITY &&
                right.value_type == s21_NEGATIVE_INFINITY)) {
        result = 0;
    } else if ((left.value_type == s21_NORMAL_VALUE) &&
               (right.value_type == s21_NORMAL_VALUE)) {
        if (leftsign < rightsign) {
            result = 1;
        } else if (leftsign > rightsign) {
            result = -1;
        } else {
            s21_scale_align(&left, &right);
            for (int i = 95; i >= 0; i--) {
                if (s21_get_bit(left, i) != s21_get_bit(right, i)) {
                    result = s21_get_bit(left, i) - s21_get_bit(right, i);
                    break;
                }
            }
        }
    }
    return result;
}

s21_decimal s21_division_by_10(s21_decimal x) {
    s21_decimal ten, res;
    s21_init_decimal(&ten);
    ten.bits[0] = 10;
    s21_init_decimal(&res);
    s21_div_bits(x, ten, &res);
    return res;
}

int s21_rem_mod_10(s21_decimal x) {
    s21_decimal res = x;
    s21_decimal ten, rem;
    s21_init_decimal(&ten);
    ten.bits[0] = 10;
    s21_init_decimal(&rem);
    res = s21_div_bits(res, ten, &rem);
    return res.bits[0];
}

int s21_is_zero(s21_decimal x) {
    int ret = 1;
    for (int i = 0; i < 3; i++) {
        if (x.bits[i] > 0) {
            ret = 0;
        }
    }
    return ret;
}

int s21_from_string_to_decimal(char *value, s21_decimal *result) {
    int conversion = SUCCESS;
    if (result && value && *value != 0) {
        s21_decimal ten, temp;
        s21_init_decimal(result);
        s21_init_decimal(&ten);
        s21_init_decimal(&temp);
        ten.bits[0] = 10u;
        int sign = 0;
        if (*value == '-') {
            sign = 1;
            value++;
        }
        int len = 0;
        char *p = value;
        for (; *p; p++) len++;
        if (len < 35) {
            p = value;
            int exp = 0;
            while (*p) {
                if (*p == 'e') break;
                exp++;
                p++;
            }
            p = value;
            int f = 0;
            while (*p) {
                if (*p == '.') break;
                f++;
                p++;
            }
            if (f == len) f = 0;
            exp = len - (len - exp);
            for (int i = 0; i < exp; i++) {
                if (value[i] == '.') continue;
                s21_from_int_to_decimal(value[i] - 48, &temp);
                *result = s21_add(*result, temp);
                if (i != exp - 1) *result = s21_mul(*result, ten);
            }
            int scale = 0;
            if (exp != len - 1) {
                for (int i = exp + 1; i < len; i++) {
                    scale += value[i] - 48;
                    if (i != len - 1) scale *= 10;
                }
            }
            if (scale < 29) {
                s21_set_scale(result, scale + (f ? (exp - f) - 1 : 0));
                if (sign) s21_set_sign(result);
            } else {
                conversion = CONVERTING_ERROR;
            }
        } else {
            conversion = CONVERTING_ERROR;
        }
    } else {
        conversion = CONVERTING_ERROR;
    }
    return conversion;
}

int s21_from_decimal_to_string(s21_decimal decimal, char *string) {
    int len = 0;
    int result = 0;
    s21_decimal temp = decimal;
    int num_count = 0;
    while (!s21_is_zero(temp)) {
        num_count++;
        s21_decimal dividend, divisor, quotient;
        dividend = temp;
        s21_init_decimal(&divisor);
        s21_init_decimal(&quotient);
        s21_from_int_to_decimal(10, &divisor);
        s21_div_bits(dividend, divisor, &quotient);
        temp = quotient;
    }
    int scale = s21_get_scale(decimal);
    len = num_count;
    if (len < scale) {
        len = len + (scale - len);
    }
    if (scale) {
        if (scale == len) {
            len++;  // для '0'
        }
        len++;
    }
    int sign = s21_get_sign(decimal);
    if (sign) {
        s21_clear_sign(&decimal);
        len++;  // для '-'
    }
    char *p = string + len;
    result = len;
    *p = 0;
    p--;
    temp = decimal;
    int point = len - scale - 1;
    while (!s21_is_zero(temp) || (p - string) >= point) {
        s21_decimal dividend, divisor, quotient, mod;
        dividend = temp;
        s21_init_decimal(&divisor);
        s21_init_decimal(&quotient);
        s21_from_int_to_decimal(10, &divisor);
        mod = s21_div_bits(dividend, divisor, &quotient);
        *p = '0' + mod.bits[0] % 10;
        p--;
        if (scale && (p - string) == point) {
            *p = '.';
            p--;
            point = -1;
        }
        temp = quotient;
    }
    if (s21_is_zero(temp) && (p - string) == 0 && scale == 28) {
        *p = '0';
    }
    if (sign) {
        string[0] = '-';
    }
    return result;
}


s21_decimal dec_div_by_ten(s21_decimal src) {
  s21_decimal tmp;
  s21_decimal res;
  s21_decimal ten;
  unsigned int tmp_bit = 1;
  s21_init_decimal(&tmp);
  s21_init_decimal(&res);
  s21_init_decimal(&ten);
  ten.bits[0] = 10;
  int is_even = 0;
  for (int i = 2, j = 0; i >= 0; i--, j++) {
    unsigned int src_bit = 0x80000000;
    while (src_bit) {
      unsigned int shift = src.bits[i] & src_bit;
      s21_shift_left(&tmp);
      if (src_bit == shift) {
        tmp.bits[0] |= tmp_bit;
      }
      if (tmp.bits[0] >= 10) {
        s21_shift_left(&res);
        res.bits[0] |= tmp_bit;
        tmp = s21_small_sub(tmp, ten);
        is_even = 0;
      } else {
        s21_shift_left(&res);
        is_even = 1;
      }
      src_bit >>= 1;
    }
  }
  s21_decimal s21_one;
  s21_init_decimal(&s21_one);
  s21_one.bits[0] = 1;
  if (tmp.bits[0] > 0 && !is_even) {
    if (tmp.bits[0] <= 4) {
      res = s21_small_sub(res, s21_one);
    } else {
      res = s21_alg_add(res, s21_one);
    }
  }
  return res;
}

s21_decimal s21_small_sub(s21_decimal x, s21_decimal y) {
  s21_decimal res;
  y = s21_invert(y);
  res = s21_alg_add(x, y);
  return res;
}

s21_decimal s21_small_mul(s21_decimal x) {
    s21_decimal result;
    s21_init_decimal(&result);
    int ten = 10;
    x.bits[3] = 0;
    while (ten != 0) {
        int mask = ten & MIN_BIT;
        if (mask == MIN_BIT) {
          int temp = 0;
          for (int i = 0; i < 4; i++) {
            unsigned int res = 0;
            for (int k = 32, shift = 1; k > 0; k--, shift <<= 1) {
              int x_sh = result.bits[i] & shift;
              int y_sh = x.bits[i] & shift;
              if ((!x_sh) && y_sh && (!temp)) {
                res = res | shift;
              } else if (x_sh && (!y_sh) && (!temp)) {
                res = res | shift;
              } else if (x_sh && y_sh && (!temp)) {
                temp++;
              } else if ((!x_sh) && (!y_sh) && temp) {
                res = res | shift;
                temp = 0;
              } else if (x_sh && y_sh && temp) {
                res = res | shift;
              }
            }
            result.bits[i] = res;
          }
        }
        ten >>= 1;
        s21_big_shift_left(&x);
    }
    return result;
}

s21_decimal *s21_big_shift_left(s21_decimal *x) {
    for (int i = 3; i >= 0; i--) {
        if (i != 3) {
            unsigned int temp = x->bits[i] & MAX_BIT;
            x->bits[i] <<= MIN_BIT;
            if (temp == MAX_BIT) {
                x->bits[i + 1] |= MIN_BIT;
            }
        } else {
            x->bits[i] <<= MIN_BIT;
        }
    }
    return x;
}
