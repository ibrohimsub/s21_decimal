#include "s21_decimal.h"

// Get a bit from s21_decimal at the given index.
int s21_getBit(s21_decimal numDec, int index) {
  int mask = 1u << (index % 32);
  return (numDec.bits[index / 32] & mask) ? 1 : 0;
}

// Get the scale from s21_decimal.
int s21_getScale(s21_decimal numDec) {
  int mask = 127 << 16;
  int scale = (mask & numDec.bits[3]) >> 16;
  return scale;
}

// Get the sign from s21_decimal.
int s21_getSign(s21_decimal numDec) {
  return (numDec.bits[3] & (1u << 31)) ? 1 : 0;
}

// Set a bit in s21_decimal at the given index.
void s21_setBit(s21_decimal *numDec, int index, int bit) {
  int mask = 1u << (index % 32);
  numDec->bits[index / 32] = (bit == 0) ? (numDec->bits[index / 32] & ~mask)
                                        : (numDec->bits[index / 32] | mask);
}

// Set the scale of s21_decimal.
void s21_setScale(s21_decimal *numDec, int scale) {
  int sign = s21_getSign(*numDec);
  numDec->bits[3] = 0;
  numDec->bits[3] |= (scale << 16);
  if (sign) s21_setSign(numDec);
}

// Set the sign of s21_decimal.
void s21_setSign(s21_decimal *numDec) { numDec->bits[3] |= (1u << 31); }

// Generate a random floating point number between a and b.
float s21_rand(float a, float b) {
  float m = (float)rand() / RAND_MAX;
  float numDec = a + m * (b - a);
  return numDec;
}

void s21_copyDec(s21_decimal *numDec1, s21_decimal numDec2) {
  int i = 127;
  while (i >= 0) {
    s21_setBit(numDec1, i, s21_getBit(numDec2, i));
    i--;
  }
}

void s21_normalizeUpperScale(s21_decimal *numDec, int norm) {
  s21_decimal power_of_10 = {0};
  s21_decimal result = {0};

  s21_from_float_to_decimal(pow(10, norm), &power_of_10);
  s21_mul(*numDec, power_of_10, &result);

  int old_scale = s21_getScale(*numDec);
  int new_scale = old_scale + norm;
  s21_setScale(&result, new_scale);

  s21_copyDec(numDec, result);
}

void s21_normalize(s21_decimal *numDec1, s21_decimal *numDec2) {
  int norm = s21_getScale(*numDec1) - s21_getScale(*numDec2);

  while (norm != 0) {
    if (norm > 0) {
      s21_normalizeUpperScale(numDec2, norm);
    } else {
      s21_normalizeUpperScale(numDec1, -norm);
    }
    norm = s21_getScale(*numDec1) - s21_getScale(*numDec2);
  }
}

// Set s21_decimal to zero.
void s21_setZero(s21_decimal *numDec) {
  numDec->bits[0] = 0;
  numDec->bits[1] = 0;
  numDec->bits[2] = 0;
  numDec->bits[3] = 0;
}

// s21_decimal s21_abs(s21_decimal dec) {
//   s21_decimal abs_dec = dec;
//   s21_setBit(&abs_dec, 127, 0);
//   return abs_dec;
// }

// void s21_setScale(s21_big_decimal *numDec, int scale) {
//   numDec->scale = scale;
// }
