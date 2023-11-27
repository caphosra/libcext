#include "cpoly/option.h"

#include "cpoly/test/option.h"

#include <assert.h>
#include <string.h>

typedef OPTION(int) OptionInt;
typedef OPTION(float) OptionFloat;

OptionInt always_some(int val) {
    int* mem = malloc(sizeof(int));
    *mem = val;
    return SOME(mem, OptionInt);
}

OptionInt always_none() {
    return NONE(OptionInt);
}

void test_option_some_none() {
    OptionInt op1 = always_some(42);

    assert(op1->kind == OPTION_SOME);
    assert(*op1->some == 42);

    OptionInt op2 = always_none();
    assert(op2->kind == OPTION_NONE);
}

OptionFloat some_unwrap_op() {
    HANDLE_NONE(OptionFloat);

    int* res = UNWRAP_OP(always_some(42));

    float* mem = malloc(sizeof(int));
    *mem = 3.14F;
    return SOME(mem, OptionFloat);
}

OptionFloat none_unwrap_op() {
    HANDLE_NONE(OptionFloat);

    int* res = UNWRAP_OP(always_none());

    return NONE(OptionFloat);
}

void test_option_unwrap_op() {
    OptionFloat op1 = some_unwrap_op();
    assert(op1->kind == OPTION_SOME);
    assert(*op1->some == 3.14F);

    OptionFloat op2 = none_unwrap_op();
    assert(op2->kind == OPTION_NONE);
}
