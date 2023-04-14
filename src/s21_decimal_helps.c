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

int s21_getExp(s21_decimal value) { return (value.bits[3] & 0x00FF0000) >> 16; }

// int s21_powDec(s21_decimal *value, int n) {
//   s21_decimal result;
//   s21_decimal ten;
//   s21_from_float_to_decimal(10, &ten);
//   s21_setZero(&result);
//   for (int i = 0; i < n; i++) {
//     s21_mul(*value, ten, &result);
//     s21_copy_to_buffer(result, value);
//   }

//   return 0;
// }

// int s21_cp(s21_decimal value, s21_decimal *dest) {
//   dest->bits[0] = value.bits[0];
//   dest->bits[1] = value.bits[1];
//   dest->bits[2] = value.bits[2];
//   dest->bits[3] = value.bits[3];
//   return 0;
// }

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

void s21_zeroBigDec(s21_big_decimal *numDec) {
  for (int i = 0; i < 8; i++) {
    numDec->bits[i] = 0;
  }
}

void s21_increaseScaleBigDec(s21_big_decimal *numDec, int n) {
  s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}}, tmp = {0};
  for (int i = 0; i < n; i++) {
    s21_mulBigDec(*numDec, ten, &tmp);
    *numDec = tmp;
    s21_zeroBigDec(&tmp);
  }
}

void s21_decreaceScaleBigDec(s21_big_decimal *numDec, int n) {
  s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}}, tmp = {0};
  for (int i = 0; i < n; i++) {
    s21_divBigDec(*numDec, ten, &tmp);
    *numDec = tmp;
    s21_zeroBigDec(&tmp);
  }
}

int s21_GreaterBigDec(s21_big_decimal value_1, s21_big_decimal value_2) {
  int result = 0, out = 0;
  for (int i = 7; i >= 0 && !result && !out; i--) {
    if (value_1.bits[i] || value_2.bits[i]) {
      if (value_1.bits[i] > value_2.bits[i]) {
        result = 1;
      }
      if (value_1.bits[i] != value_2.bits[i]) out = 1;
    }
  }
  return result;
}

int s21_is_greater_or_equal_big_decimal(s21_big_decimal value_1,
                                        s21_big_decimal value_2) {
  int result = 0, out = 0;
  for (int i = 7; i >= 0 && !out && !result; i--) {
    if (value_1.bits[i] != 0 || value_2.bits[i] != 0) {
      if (value_1.bits[i] >= value_2.bits[i]) {
        result = 1;
      }
      out = 1;
    }
  }
  return result;
}

int s21_DecNotEmpty(s21_decimal numDec) {
  return numDec.bits[0] + numDec.bits[1] + numDec.bits[2];
}

int s21_BigDecNotEmpty(s21_big_decimal Decnum) {
  return Decnum.bits[0] + Decnum.bits[1] + Decnum.bits[2] + Decnum.bits[3] +
         Decnum.bits[4] + Decnum.bits[5] + Decnum.bits[6] + Decnum.bits[7];
}

void s21_srcHighestBitBigDec(s21_big_decimal t1, s21_big_decimal t2, int *bit_1,
                             int *bit_2) {
  for (int i = 255; i >= 0 && (!(*bit_1) || !(*bit_2)); i--) {
    if (*bit_1 == 0 && s21_getBigBit(t1, i)) *bit_1 = i;
    if (*bit_2 == 0 && s21_getBigBit(t2, i)) *bit_2 = i;
  }
}

int s21_equBitsBigDec(s21_big_decimal *value_1, s21_big_decimal *value_2) {
  int res = 0;
  // while (s21_GreaterBigDec(*value_2, *value_1)) {
  //   s21_increaseScaleBigDec(value_1, 1);
  //   res++;
  // }
  while (s21_is_greater_or_equal_big_decimal(*value_1, *value_2)) {
    s21_moveBigDecLeft(value_2, 1);
  }
  s21_moveBigDecRight(value_2, 1);
  return res;
}

// s21_decimal s21_abs(s21_decimal dec) {
//   s21_decimal abs_dec = dec;
//   s21_setBit(&abs_dec, 127, 0);
//   return abs_dec;
// }

// void s21_setScale(s21_big_decimal *numDec, int scale) {
//   numDec->scale = scale;
// }
