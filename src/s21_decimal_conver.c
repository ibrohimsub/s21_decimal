#include "s21_decimal.h"

int s21_from_int_to_decimal(int number, s21_decimal *numDec) {
  s21_setZero(numDec);
  int fail = (number > INT_MAX) ? 1 : 0;
  numDec->bits[0] = (number < 0) ? ((s21_setSign(numDec), -number)) : number;
  return fail;
}

int s21_from_float_to_decimal(float number, s21_decimal *numDec) {
  s21_setZero(numDec);
  int return_value = 0;

  if (isinf(number) || isnan(number)) {
    return_value = 1;
  } else {
    if (number != 0) {
      // Extract the sign, exponent, and mantissa bits from the float.
      int sign = *(int *)&number >> 31;
      int exp = ((*(int *)&number & ~0x80000000) >> 23) - 127;
      double temp = (double)fabs(number);
      int off = 0;

      // Calculate the offset and round the input value.
      for (; off < 28 && (int)temp / (int)pow(2, 21) == 0; temp *= 10, off++) {
      }
      temp = round(temp);

      // Convert the float to a decimal.
      if (off <= 28 && (exp > -94 && exp < 96)) {
        floatbits mant = {0};
        temp = (float)temp;

        // Remove trailing zeros from the input value.
        for (; fmod(temp, 10) == 0 && off > 0; off--, temp /= 10) {
        }

        // Extract the mantissa bits.
        mant.fl = temp;
        exp = ((*(int *)&mant.fl & ~0x80000000) >> 23) - 127;

        // Set the appropriate bit in the decimal for each non-zero mantissa
        // bit.
        numDec->bits[exp / 32] |= 1 << exp % 32;
        for (int i = exp - 1, j = 22; j >= 0; i--, j--) {
          if ((mant.ui & (1 << j)) != 0) {
            numDec->bits[i / 32] |= 1 << i % 32;
          }
        }

        // Set the sign and offset bits in the decimal.
        numDec->bits[3] = (sign << 31) | (off << 16);
      }
    }
  }

  return return_value;
}

int s21_from_decimal_to_float(s21_decimal number, float *numDec) {
  double decPart = 0.0;
  for (int i = 0; i < 96; i++) {
    decPart += s21_getBit(number, i) * pow(2, i);
  }
  decPart = decPart * pow(10, -s21_getScale(number));

  if (s21_getBit(number, 127)) {
    decPart *= -1;
  }

  *numDec = (float)decPart;

  return 0;
}

int s21_from_decimal_to_int(s21_decimal number, int *numDec) {
  int fail = 0;
  int scale = s21_getScale(number);
  if (number.bits[1] != 0 || number.bits[2] != 0) {
    fail = 1;  // Too many bits in the decimal part
  } else {
    *numDec = number.bits[0];
    if (scale > 0 && scale <= 28) {
      // Shift decimal point to the left
      *numDec /= pow(10, scale);
    }
  }
  if (s21_getSign(number) != 0) {
    // Set the sign of the result
    *numDec = -(*numDec);
  }
  return fail;
}
