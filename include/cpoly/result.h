#pragma once

#include "cpoly/type.h"

#include <stdlib.h>
#include <setjmp.h>

///
/// Represents a kind of RESULT.
///
typedef enum {
    RESULT_OK,
    RESULT_ERR,
} ResultKind;

///
/// A result that consists of untyped items.
///
typedef struct {
    ResultKind kind;
    union {
        void* ok;
        void* err;
    };
} ResultBase;

ResultBase* __cpoly_ok(void* ok);
ResultBase* __cpoly_err(void* err);
void* __cpoly_unwrap(ResultBase* result, volatile void** err_obj, jmp_buf* err_jmp_point);

///
/// Gets the dereferenced type of RESULT.
/// In most cases, it would be better to use RESULT.
///
#define RESULT_DEREF(OKType, ErrType) \
    struct { \
        ResultKind kind; \
        union { \
            OKType* ok; \
            ErrType* err; \
        }; \
    }

///
/// Gets the type of RESULT.
///
#define RESULT(OKType, ErrType) RESULT_DEREF(OKType, ErrType)*

#define ASSERT_TYPE(item1, item2, ret) _Generic((item1), TYPEOF(item2): ret, default: NULL)

///
/// Casts a result to OK.
///
#define OK(item, type) \
    (type)ASSERT_TYPE(item, ((type)NULL)->ok, __cpoly_ok)((void*)item)

///
/// Casts a result to ERR.
///
#define ERR(item, type) \
   (type)ASSERT_TYPE(item, ((type)NULL)->err, __cpoly_err)((void*)item)

///
/// Handles early-return from UNWRAP.
///
#define HANDLE_ERR(type) \
    jmp_buf __cpoly_err_jmp_point; TYPEOF(((type)NULL)->err) __cpoly_err_type; volatile TYPEOF(((type)NULL)->err) __cpoly_err_obj; \
    if (setjmp(__cpoly_err_jmp_point)) \
        return ERR(__cpoly_err_obj, type)

///
/// Unwraps RESULT. If it holds an error info, exit the function early.
/// Do not forget to call HANDLE_ERR before using this.
///
#define UNWRAP(result) \
    (TYPEOF(result->ok)) \
        ASSERT_TYPE(result->err, __cpoly_err_type, __cpoly_unwrap) \
        ((ResultBase*)result, (volatile void**)&__cpoly_err_obj, &__cpoly_err_jmp_point)
