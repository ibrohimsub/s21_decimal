#ifndef _SRC_S21_DECIMAL_H_
#define _SRC_S21_DECIMAL_H_

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned bits[8];
} s21_big_decimal;

typedef union {
  int ui;
  float fl;
} floatbits;

int s21_divBigDec(s21_big_decimal value_1, s21_big_decimal value_2,
                  s21_big_decimal *result);
void s21_addBigDec(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result);
void s21_subBigDec(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result);
int s21_mulBigDec(s21_big_decimal value_1, s21_big_decimal value_2,
                  s21_big_decimal *result);

int s21_getBigBit(s21_big_decimal src, int i);
void s21_setBigBit(s21_big_decimal *numDec, int i, int bit);
int s21_moveBigDecLeft(s21_big_decimal *numDec, int num);
void s21_moveBigDecRight(s21_big_decimal *numDec, int num);
void s21_moveBigDec(s21_decimal value_1, s21_big_decimal *value_2);
void s21_moveSmallDec(s21_decimal *value_1, s21_big_decimal value_2);
void s21_zeroBigDec(s21_big_decimal *numDec);
void s21_norma(s21_big_decimal *value_1, s21_big_decimal *value_2, int diff);

// s21_decimal s21_abs(s21_decimal dec);
// void s21_set_scale2(s21_big_decimal *num, int scale);

int s21_greaterNum(int a, int b);
void s21_decComparReverse(int *flag);
int s21_decComparBits(s21_decimal first, s21_decimal second);
int s21_decCompar(s21_decimal numDec1, s21_decimal numDec2);

int s21_decDouble(s21_decimal src, long double *num);
float s21_rand(float a, float b);
void s21_setBit(s21_decimal *numDec, int i, int bit);
void s21_setScale(s21_decimal *numDec, int scale);
void s21_setSign(s21_decimal *numDec);
void s21_setZero(s21_decimal *numDec);
int s21_getBit(s21_decimal src, int i);
int s21_getScale(s21_decimal numDec);
int s21_getSign(s21_decimal numDec);

void s21_increaseScaleBigDec(s21_big_decimal *numDec, int n);
void s21_decreaceScaleBigDec(s21_big_decimal *numDec, int n);
void s21_srcHighestBitBigDec(s21_big_decimal v1, s21_big_decimal v2, int *bit_1,
                             int *bit_2);
int s21_DecNotEmpty(s21_decimal numDec);
int s21_BigDecNotEmpty(s21_big_decimal Decnum);
int s21_equBitsBigDec(s21_big_decimal *value_1, s21_big_decimal *value_2);
int s21_GreaterEqualBigDec(s21_big_decimal value_1, s21_big_decimal value_2);
int s21_GreaterBigDec(s21_big_decimal value_1, s21_big_decimal value_2);

void s21_normalize(s21_decimal *numDec1, s21_decimal *numDec2);
void s21_normalizeUpperScale(s21_decimal *numDec, int norm);
void s21_copyDec(s21_decimal *numDec1, s21_decimal numDec2);
int s21_postNorma(s21_big_decimal *result, int scale);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal dec, s21_decimal *result);

int s21_from_int_to_decimal(int src, s21_decimal *numDec);
int s21_from_float_to_decimal(float src, s21_decimal *numDec);
int s21_from_decimal_to_int(s21_decimal src, int *num);
int s21_from_decimal_to_float(s21_decimal src, float *num);

int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

#endif  // _SRC_S21_DECIMAL_H_