#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  s21_truncate(value, result);
  if (s21_getSign(value) && !s21_is_equal(*result, value)) {
    s21_sub(*result, (s21_decimal){{1, 0, 0, 0}}, result);
  }
  return 0;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  s21_setZero(result);
  long double double_value = 0.0;
  s21_decDouble(value, &double_value);
  double rounded_value = round(double_value);
  if (rounded_value < 0) {
    s21_setSign(result);
    rounded_value = -rounded_value;
  }
  for (int i = 0; i < 96; i++) {
    double_value = floor(rounded_value / 2.0);
    int bit = (rounded_value - 2.0 * double_value >= 1.0) ? 1 : 0;
    s21_setBit(result, i, bit);
    rounded_value = double_value;
  }
  s21_setScale(result, 0);
  return 0;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  s21_setZero(result);

  if (s21_getScale(value)) {
    long double fl_to_int = 0.0;
    s21_decDouble(value, &fl_to_int);
    fl_to_int = trunc(fl_to_int);

    if (fl_to_int < 0.0) {
      s21_setBit(result, 127, 1);
      fl_to_int *= -1;
    }

    for (int i = 0; fl_to_int >= 1 && i < 96; i++) {
      fl_to_int = floor(fl_to_int) / 2;

      if (fmod(fl_to_int, 1.0) != 0.0) {
        s21_setBit(result, i, 1);
      } else {
        s21_setBit(result, i, 0);
      }
    }

    s21_setScale(result, 0);
  } else {
    for (int i = 0; i < 4; ++i) {
      result->bits[i] = value.bits[i];
    }
  }

  return 0;
}

int s21_decDouble(s21_decimal value, long double *result) {
  long double tmp = (double)*result;  // Cast to long double
  for (int i = 0; i < 96; i++) {
    int bit = s21_getBit(value, i);  // Get the i-th bit of the value
    tmp += bit * pow(2, i);  // Add the value of the bit to the tmp value
  }
  int scale = s21_getScale(value);  // Get the scale of the value
  tmp = tmp * pow(10, -scale);      // Apply the scale factor to the tmp value
  if (s21_getBit(value, 127)) {     // Check if the sign bit is set
    tmp = tmp * (-1);               // Negate the tmp value if sign bit is set
  }
  *result = tmp;  // Assign the final value to the result pointer
  return 0;       // Return success
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  s21_setZero(result);
  *result = value;

  // Flip the sign bit
  s21_setBit(result, 127, !s21_getSign(value));

  return 0;
}
