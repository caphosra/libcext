#include "modc/result.h"

#include "modc/test/result.h"

#include <assert.h>
#include <string.h>

IMPL_RESULT(int, char)
IMPL_RESULT(float, char)

RESULT(int, char) always_ok(int val) {
    int* mem = malloc(sizeof(int));
    *mem = val;
    return OK(int, char)(mem);
}

RESULT(int, char) always_err() {
    char* val = calloc(0x100, sizeof(char));
    strcpy(val, "Why don't we use Rust?");

    return ERR(int, char)(val);
}

void test_result_ok_err() {
    RESULT(int, char) res1 = always_ok(42);

    assert(res1->kind == MODC_RESULT_OK);
    assert(*res1->ok == 42);

    RESULT(int, char) res2 = always_err();
    assert(res2->kind == MODC_RESULT_ERR);
    assert(!strcmp(res2->err, "Why don't we use Rust?"));
}

RESULT(float, char) ok_unwrap() {
    HANDLE_ERR(float, char);

    int* res = UNWRAP(always_ok(42));

    float* mem = malloc(sizeof(int));
    *mem = 3.14F;
    return OK(float, char)(mem);
}

RESULT(float, char) err_unwrap() {
    HANDLE_ERR(float, char);

    int* res = UNWRAP(always_err());

    float* mem = malloc(sizeof(float));
    *mem = 3.14F;
    return OK(float, char)(mem);
}

void test_result_unwrap() {
    RESULT(float, char) res1 = ok_unwrap();
    assert(res1->kind == MODC_RESULT_OK);
    assert(*res1->ok == 3.14F);

    RESULT(float, char) res2 = err_unwrap();
    assert(res2->kind == MODC_RESULT_ERR);
    assert(!strcmp(res2->err, "Why don't we use Rust?"));
}
