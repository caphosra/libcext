#include "cpoly/result.h"

#include "cpoly/test/result.h"

#include <assert.h>
#include <string.h>

typedef RESULT(int, char) ResultIntChar;
typedef RESULT(float, char) ResultFloatChar;

ResultIntChar always_ok(int val) {
    int* mem = malloc(sizeof(int));
    *mem = val;
    return OK(mem, ResultIntChar);
}

ResultIntChar always_err() {
    char* val = calloc(0x100, sizeof(char));
    strcpy(val, "Why don't we use Rust?");

    return ERR(val, ResultIntChar);
}

void test_result_ok_err() {
    ResultIntChar res1 = always_ok(42);

    assert(res1->kind == RESULT_OK);
    assert(*res1->ok == 42);

    ResultIntChar res2 = always_err();
    assert(res2->kind == RESULT_ERR);
    assert(!strcmp(res2->err, "Why don't we use Rust?"));
}

ResultFloatChar ok_unwrap() {
    HANDLE_ERR(ResultFloatChar);

    int* res = UNWRAP(always_ok(42));

    float* mem = malloc(sizeof(int));
    *mem = 3.14F;
    return OK(mem, ResultFloatChar);
}

ResultFloatChar err_unwrap() {
    HANDLE_ERR(ResultFloatChar);

    int* res = UNWRAP(always_err());

    float* mem = malloc(sizeof(float));
    *mem = 3.14F;
    return OK(mem, ResultFloatChar);
}

void test_result_unwrap() {
    ResultFloatChar res1 = ok_unwrap();
    assert(res1->kind == RESULT_OK);
    assert(*res1->ok == 3.14F);

    ResultFloatChar res2 = err_unwrap();
    assert(res2->kind == RESULT_ERR);
    assert(!strcmp(res2->err, "Why don't we use Rust?"));
}
