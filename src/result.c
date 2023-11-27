#include "cpoly/result.h"

#include <setjmp.h>

ResultBase* __cpoly_ok(void* ok) {
    ResultBase* result = calloc(1, sizeof(ResultBase));
    result->kind = RESULT_OK;
    result->ok = ok;
    return result;
}

ResultBase* __cpoly_err(void* err) {
    ResultBase* result = calloc(1, sizeof(ResultBase));
    result->kind = RESULT_ERR;
    result->err = err;
    return result;
}

void* __cpoly_unwrap(ResultBase* result, volatile void** err_obj, jmp_buf* err_jmp_point) {
    if (result->kind == RESULT_OK)
        return result->ok;
    *err_obj = result->err;
    longjmp(*err_jmp_point, 1);
    return NULL;
}
