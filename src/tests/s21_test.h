#ifndef SRC_S21_DECIMAL_TEST_H_
#define SRC_S21_DECIMAL_TEST_H_

#include <check.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include "../s21_decimal.h"

#define EXP_PLUS_1 65536
#define EXP_PLUS_2 196608
#define EXP_PLUS_28 1835008

void tests(void);
void testCase(Suite *test_case);

#endif  //  SRC_S21_DECIMAL_TEST_H_
