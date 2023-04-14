#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *numDec) {
  int return_value = 0;

  s21_setZero(numDec);
  if (isinf(src) || isnan(src)) {
    return_value = 1;
  } else {
    if (src != 0) {
      // Extract the sign, exponent, and mantissa bits from the float.
      int sign = *(int *)&src >> 31;
      int exp = ((*(int *)&src & ~0x80000000) >> 23) - 127;
      double temp = fabs(src);
      int off = 0;

      // Calculate the offset and round the input value.
      while (off < 28 && (int)temp / (int)pow(2, 21) == 0) {
        temp *= 10;
        off++;
      }
      temp = round(temp);

      // Convert the float to a decimal.
      if (off <= 28 && (exp > -94 && exp < 96)) {
        float mant = temp;

        // Remove trailing zeros from the input value.
        while (fmod(mant, 10) == 0 && off > 0) {
          mant /= 10;
          off--;
        }

        // Extract the mantissa bits.
        int mant_bits = *(int *)&mant;
        exp = ((mant_bits & ~0x80000000) >> 23) - 127;

        // Set the appropriate bit in the decimal for each non-zero mantissa
        // bit.
        numDec->bits[exp / 32] |= 1 << (exp % 32);
        for (int i = exp - 1, j = 22; j >= 0; i--, j--) {
          if ((mant_bits & (1 << j)) != 0) {
            numDec->bits[i / 32] |= 1 << (i % 32);
          }
        }

        // Set the sign and offset bits in the decimal.
        numDec->bits[3] = (sign << 31) | (off << 16);
      }
    }
  }

  return return_value;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  s21_setZero(dst);
  int fail = (src > INT_MAX) ? 1 : 0;
  dst->bits[0] = (src < 0) ? ((s21_setSign(dst), -src)) : src;
  return fail;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int status = 0;
  *dst = 0;
  if (dst == NULL) {
    status = 1;
  } else {
    int minus = 1;
    long double result = 0, two = 1;
    int exp = s21_getExp(src);
    if (s21_getSign(src) == 1) minus = -1;
    for (int k = 0; k < 96; k++) {
      if (s21_getBit(src, k)) {
        result += two;
      }
      two *= 2;
    }
    for (int k = 0; k < exp; k++) {
      result /= 10;
    }
    *dst = minus * result;
  }
  return status;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int fail = 0;
  int scale = s21_getScale(src);
  if (src.bits[1] != 0 || src.bits[2] != 0) {
    fail = 1;  // Too many bits in the decimal part
  } else {
    *dst = src.bits[0];
    if (scale > 0 && scale <= 28) {
      // Shift decimal point to the left
      *dst /= pow(10, scale);
    }
  }
  if (s21_getSign(src) != 0) {
    // Set the sign of the result
    *dst = -(*dst);
  }
  return fail;
}
