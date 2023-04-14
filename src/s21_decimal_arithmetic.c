#include "s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int fail = 0;
  if (result != NULL) {
    // Swap operands if necessary
    if (s21_getSign(value_1) && s21_getSign(value_2)) {
      s21_decimal temporary1 = value_1;
      value_1 = value_2;
      value_2 = temporary1;
      s21_setBit(&value_1, 127, 0);
      s21_setBit(&value_2, 127, 0);
    }

    int res = 0, last_sign = 0;
    // Compute sign of the result
    if (s21_getSign(value_1) != s21_getSign(value_2)) {
      s21_getSign(value_1) ? last_sign = 1 : 1;
      s21_setBit(&value_1, 127, 0);
      s21_setBit(&value_2, 127, 0);
      fail = s21_add(value_1, value_2, result);
    } else {
      // Compute difference of the values using big decimal arithmetic
      s21_big_decimal t1 = {0}, t2 = {0}, res_diff = {0};
      s21_moveBigDec(value_1, &t1);
      s21_moveBigDec(value_2, &t2);
      int diff = s21_getScale(value_1) - s21_getScale(value_2);
      diff > 0 ? s21_setScale(&value_2, s21_getScale(value_2) + diff)
               : s21_setScale(&value_1, s21_getScale(value_1) - diff);
      s21_norma(&t1, &t2, diff);
      if (s21_GreaterBigDec(t2, t1)) {
        s21_big_decimal temporary2 = t1;
        t1 = t2;
        t2 = temporary2;
        s21_setSign(result);
      }
      s21_subBigDec(t1, t2, &res_diff);
      res = s21_postNorma(&res_diff, s21_getScale(value_1));
      if (res >= 0) {
        s21_moveSmallDec(result, res_diff);
        s21_setScale(result, res);
      } else {
        fail = 1;
      }
    }

    // Set the sign of the result
    last_sign == 1 ? s21_setSign(result) : 0;

    // Check for overflow and underflow
    if (fail == 1 && s21_getSign(*result)) fail = 2;

    // Set result to zero if there was an fail
    if (fail) s21_setZero(result);
  } else {
    fail = 1;
  }
  return fail;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int fail = 0;

  if (result != NULL) {
    int value_1_sign = s21_getSign(value_1);
    int value_2_sign = s21_getSign(value_2);

    int last_sign = 0;
    if (value_1_sign && value_2_sign) {
      last_sign = 1;
    }

    int fail = 0;
    int take = 0;
    if (value_1_sign != value_2_sign) {
      int sign = value_1_sign;
      s21_setBit(&value_1, 127, 0);
      s21_setBit(&value_2, 127, 0);
      if (sign) {
        fail = s21_sub(value_2, value_1, result);
      } else {
        fail = s21_sub(value_1, value_2, result);
      }
    } else {
      s21_big_decimal t1 = {0}, t2 = {0}, res_diff = {0};
      int res = 0;
      s21_moveBigDec(value_1, &t1);
      s21_moveBigDec(value_2, &t2);
      int diff = s21_getScale(value_1) - s21_getScale(value_2);
      if (diff > 0) {
        take = s21_getScale(value_1);
        s21_setScale(&value_2, take);
      } else {
        take = s21_getScale(value_2);
        s21_setScale(&value_1, take);
      }
      s21_norma(&t1, &t2, diff);
      s21_addBigDec(t1, t2, &res_diff);
      res = s21_postNorma(&res_diff, s21_getScale(value_1));
      if (res >= 0) {
        s21_moveSmallDec(result, res_diff);
        s21_setScale(result, res);
      } else {
        fail = 1;
      }
    }

    if (last_sign == 1) {
      s21_setSign(result);
    }

    if (fail == 1 && s21_getSign(*result)) {
      fail = 2;
    }

    if (fail) {
      s21_setZero(result);
    }
  } else {
    fail = 1;
  }

  return fail;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int fail = 0;

  if (result != NULL) {
    s21_big_decimal t1 = {0}, t2 = {0}, res_diff = {0};
    s21_moveBigDec(value_1, &t1);
    s21_moveBigDec(value_2, &t2);

    // Determine sign of result
    if (s21_getSign(value_1) != s21_getSign(value_2)) {
      s21_setSign(result);
    }

    // Compute product of absolute values
    fail = s21_mulBigDec(t1, t2, &res_diff);

    int res = 0;
    // Compute res of result and normalize
    res = s21_getScale(value_1) + s21_getScale(value_2);
    res = s21_postNorma(&res_diff, res);

    // Check for overflow and set result
    if (res >= 0) {
      s21_setScale(result, res);
      s21_moveSmallDec(result, res_diff);
    } else {
      fail = 1;
    }

    // Check for negative zero and set fail code
    if (fail == 1 && s21_getSign(*result)) {
      fail = 2;
    }

    // Set result to zero in case of fail
    if (fail) {
      s21_setZero(result);
    }
  } else {
    fail = 1;
  }

  return fail;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int fail = 0;
  if (result != NULL) {
    if (s21_DecNotEmpty(value_2)) {
      int res = 0, res_scale = 0;
      s21_big_decimal t1 = {0}, t2 = {0}, res_diff = {0};
      s21_moveBigDec(value_1, &t1);
      s21_moveBigDec(value_2, &t2);
      if (s21_getSign(value_1) != s21_getSign(value_2)) s21_setSign(result);
      res = s21_divBigDec(t1, t2, &res_diff);
      s21_setScale(&value_1, s21_getScale(value_1) + res);
      res_scale = s21_getScale(value_1) - s21_getScale(value_2);
      if (res_scale > 0) {
        res_scale = s21_postNorma(&res_diff, res_scale);
      } else if (res_scale < 0) {
        s21_increaseScaleBigDec(&res_diff, abs(res_scale));
        res_scale = s21_postNorma(&res_diff, 0);
      }
      if (res_scale >= 0) {
        s21_moveSmallDec(result, res_diff);
        s21_setScale(result, res_scale);
      } else {
        fail = 1;
      }
    } else {
      fail = 3;
    }
    if (fail == 1 && s21_getSign(*result)) {
      fail = 2;
    }

    if (fail) {
      s21_setZero(result);
    }
  } else {
    fail = 1;
  }

  return fail;
}

int s21_divBigDec(s21_big_decimal value_1, s21_big_decimal value_2,
                  s21_big_decimal *result) {
  int b_1 = 0, b_2 = 0, bit_2 = 0, res = 0, diff = 0, save_scale = 0;
  s21_big_decimal tmp = {0};

  s21_srcHighestBitBigDec(value_1, value_2, &b_1, &b_2);
  bit_2 = b_2;

  for (int i = 0; i < 96 && s21_BigDecNotEmpty(value_1);) {
    if (i > 0) {
      s21_moveBigDecLeft(&value_2, 1);
      s21_increaseScaleBigDec(result, 1);
      s21_increaseScaleBigDec(&value_1, 1);
      save_scale++;
    }

    res = s21_equBitsBigDec(&value_1, &value_2);
    save_scale += res;

    b_1 = b_2 = 0;
    s21_srcHighestBitBigDec(value_1, value_2, &b_1, &b_2);
    diff = b_2 - bit_2;
    diff = diff < 0 ? 0 : diff;

    while (diff >= 0 && s21_BigDecNotEmpty(value_1)) {
      if (s21_GreaterBigDec(value_2, value_1)) {
        s21_setBigBit(&tmp, 0, 0);
      } else {
        s21_subBigDec(value_1, value_2, &value_1);
        s21_setBigBit(&tmp, 0, 1);
      }

      i++;
      diff--;

      if (diff >= 0) {
        s21_moveBigDecRight(&value_2, 1);
      }

      s21_moveBigDecLeft(&tmp, 1);
    }

    if (diff >= 0) {
      s21_moveBigDecLeft(&tmp, diff + 1);
    }

    s21_moveBigDecRight(&tmp, 1);
    s21_addBigDec(*result, tmp, result);
    s21_zeroBigDec(&tmp);
  }

  return save_scale;
}

void s21_moveBigDec(s21_decimal value_1, s21_big_decimal *value_2) {
  for (int i = 0; i < 3; i++) {
    value_2->bits[i] = value_1.bits[i];
  }
}

void s21_moveSmallDec(s21_decimal *value_1, s21_big_decimal value_2) {
  for (int i = 0; i < 3; i++) {
    value_1->bits[i] = value_2.bits[i];
  }
}

void s21_setBigBit(s21_big_decimal *numDec, int i, int bit) {
  int show = 1u << (i % 32);
  numDec->bits[i / 32] = (bit == 0) ? (numDec->bits[i / 32] & ~show)
                                    : (numDec->bits[i / 32] | show);
}

int s21_getBigBit(s21_big_decimal numDec, int i) {
  int show = 1u << (i % 32);
  return ((numDec.bits[i / 32] & show) != 0) ? 1 : 0;
}

void s21_moveBigDecRight(s21_big_decimal *numDec, int number) {
  int dat[7] = {0};

  for (int k = 0; k < number; k++) {
    // Copy the highest bit of each 32-bit word to a dat
    for (int i = 0; i < 7; i++) {
      dat[i] = s21_getBigBit(*numDec, (i + 1) * 32);
    }

    // Shift each 32-bit word right by 1 bit and set the highest bit to the
    // corresponding value from the dat
    for (int i = 0; i < 7; i++) {
      numDec->bits[i] >>= 1;
      s21_setBigBit(numDec, (i + 1) * 32 - 1, dat[i]);
    }

    // Shift the highest 32-bit word right by 1 bit
    numDec->bits[7] >>= 1;
  }
}

int s21_moveBigDecLeft(s21_big_decimal *numDec, int number) {
  int fail = 0;
  int dat[8] = {0};
  for (int k = 0; k < number; k++) {
    for (int i = 0; i < 7; i++) {
      dat[i] = s21_getBigBit(*numDec, (i + 1) * 32 - 1);
    }
    for (int i = 7; i > 0; i--) {
      numDec->bits[i] <<= 1;
      s21_setBigBit(numDec, i * 32, dat[i - 1]);
    }
    numDec->bits[0] <<= 1;
    s21_setBigBit(numDec, 0, dat[7]);
  }
  return fail;
}

void s21_norma(s21_big_decimal *value_1, s21_big_decimal *value_2, int diff) {
  if (diff > 0) {
    s21_increaseScaleBigDec(value_2, diff);
  } else if (diff < 0) {
    s21_increaseScaleBigDec(value_1, -diff);
  }
}

int s21_postNorma(s21_big_decimal *result, int res) {
  int hasSignificantBits =
      (result->bits[3] || result->bits[4] || result->bits[5] ||
       result->bits[6] || result->bits[7]);
  int hasRoundedDigit = 0;

  while (hasSignificantBits && res > 0) {
    if (res == 1 && result->bits[3]) {
      hasRoundedDigit = 1;
    }

    s21_decreaceScaleBigDec(result, 1);
    res--;

    hasSignificantBits =
        (result->bits[3] || result->bits[4] || result->bits[5] ||
         result->bits[6] || result->bits[7]);
  }

  if (hasRoundedDigit && res == 0 && result->bits[3] == 0 &&
      s21_getBigBit(*result, 0)) {
    s21_setBigBit(result, 0, 0);
  }

  if (hasSignificantBits) {
    res = -1;
  }

  return res;
}

void s21_addBigDec(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result) {
  uint64_t carry = 0;
  for (int i = 0; i < 8; i++) {
    uint64_t sum =
        (uint64_t)value_1.bits[i] + (uint64_t)value_2.bits[i] + carry;
    result->bits[i] = (uint32_t)(sum & ((1ull << 32) - 1));
    carry = sum >> 32;
  }
}

void s21_subBigDec(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result) {
  int borrow = 0;
  for (int i = 0; i < 8; i++) {
    int64_t diff = (int64_t)value_1.bits[i] - (int64_t)value_2.bits[i] - borrow;
    result->bits[i] =
        (diff >= 0) ? (unsigned int)diff : (unsigned int)(diff + (1ull << 32));
    borrow = (diff < 0);
  }
}

int s21_mulBigDec(s21_big_decimal value_1, s21_big_decimal value_2,
                  s21_big_decimal *result) {
  int fail = 0, count = 0;
  for (int i = 0; i < 256 && !fail; i++) {
    if (s21_getBigBit(value_2, i)) {
      fail = s21_moveBigDecLeft(&value_1, i - count);
      s21_addBigDec(value_1, *result, result);
      count = i;
    }
  }
  return fail;
}