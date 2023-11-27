#include "modc/result.h"

#include <setjmp.h>

ResultBase* __modc_ok(void* ok) {
    ResultBase* result = calloc(1, sizeof(ResultBase));
    result->kind = MODC_RESULT_OK;
    result->ok = ok;
    return result;
}

ResultBase* __modc_err(void* err) {
    ResultBase* result = calloc(1, sizeof(ResultBase));
    result->kind = MODC_RESULT_ERR;
    result->err = err;
    return result;
}

void* __modc_unwrap(ResultBase* result, volatile void** err_obj, jmp_buf* err_jmp_point) {
    if (result->kind == MODC_RESULT_OK)
        return result->ok;
    *err_obj = result->err;
    longjmp(*err_jmp_point, 1);
    return NULL;
}
