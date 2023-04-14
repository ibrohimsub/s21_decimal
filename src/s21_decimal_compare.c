#include "s21_decimal.h"

int s21_decComparBits(s21_decimal first, s21_decimal second) {
  for (int i = 95; i >= 0; --i) {
    int bit1 = s21_getBit(first, i);
    int bit2 = s21_getBit(second, i);
    int ret = (bit1 > bit2) ? 1 : (bit1 < bit2) ? 2 : 0;
    if (ret) {
      return ret;
    }
  }
  return 0;
}

void s21_ComparRev(int *ret) {
  *ret = (*ret == 1) ? 2 : (*ret == 2) ? 1 : *ret;
}

int s21_greater(int a, int b) { return (a > b) ? 1 : (a < b) ? 2 : 0; }

int s21_is_less(s21_decimal numDec1, s21_decimal numDec2) {
  return s21_is_greater(numDec1, numDec2) ^ 1 &&
         s21_is_equal(numDec1, numDec2) == 0;
}

int s21_is_less_or_equal(s21_decimal numDec1, s21_decimal numDec2) {
  return s21_is_less(numDec1, numDec2) || s21_is_equal(numDec1, numDec2);
}

int s21_is_greater(s21_decimal numDec1, s21_decimal numDec2) {
  return (s21_decCompar(numDec1, numDec2) == 1) ? 1 : 0;
}

int s21_is_greater_or_equal(s21_decimal numDec1, s21_decimal numDec2) {
  return s21_is_greater(numDec1, numDec2) || s21_is_equal(numDec1, numDec2);
}

int s21_is_equal(s21_decimal numDec1, s21_decimal numDec2) {
  return (s21_decCompar(numDec1, numDec2) == 0) ? 1 : 0;
}

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  return s21_is_equal(a, b) == 0;
}

int s21_decCompar(s21_decimal numDec1, s21_decimal numDec2) {
  int ret = 0;
  int scale_dif = (s21_getScale(numDec1) - s21_getScale(numDec2));
  s21_normalize(&numDec1, &numDec2);

  ret = s21_greater(!s21_getSign(numDec1), !s21_getSign(numDec2));

  if (ret == 0) {
    scale_dif >= 0
        ? ret = s21_decComparBits(numDec1, numDec2)
        : ((ret = s21_decComparBits(numDec2, numDec1)), s21_ComparRev(&ret));

    s21_getSign(numDec1) && s21_getSign(numDec2) ? s21_ComparRev(&ret) : 0;
  } else {
    (numDec1.bits[0] == 0 && numDec1.bits[1] == 0 && numDec1.bits[2] == 0 &&
     numDec2.bits[0] == 0 && numDec2.bits[1] == 0 && numDec2.bits[2] == 0)
        ? ret = 0
        : 0;
  }

  return ret;
}
