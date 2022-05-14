#ifndef _UNITTEST_H
#define _UNITTEST_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

#include "CUnit/Basic.h"

typedef struct {
    CU_TestFunc fnc;
    char label[200];
} T_FUNC_DESCRIPTOR;

extern T_FUNC_DESCRIPTOR ALL_TEST_FUNCS[];
extern char SUITE_NAME[];

#define ECHO_IF_NOT_EQUAL(actual, expected) \
    if ((actual) != (expected)) printf("%s:%i Expected: %i; Actual: %i\n", __FILE__, __LINE__, expected, actual);

#define CU_ASSERT_EQUAL_ECHO(actual, expected) \
  { ECHO_IF_NOT_EQUAL(actual, expected) CU_assertImplementation(((actual) == (expected)), __LINE__, ("CU_ASSERT_EQUAL(" #actual "," #expected ")"), __FILE__, "", CU_FALSE); }

#define DOUBLE_GRANULARITY .000002

#endif
