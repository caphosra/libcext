#pragma once

#include "modc/typeof.h"

#include <stdlib.h>
#include <setjmp.h>

///
/// Gets the dereferenced type of RESULT.
/// In most cases, it would be better to use RESULT.
///
#define RESULT_DEREF(OKType, ErrType) modc_Result_ ## OKType ## _ ## ErrType

///
/// Gets the type of RESULT.
/// Do not forget to use DEFINE_RESULT and IMPL_RESULT.
///
#define RESULT(OKType, ErrType) RESULT_DEREF(OKType, ErrType)*

///
/// Returns a function that casts result to OK.
///
#define OK(OKType, ErrType) \
    modc_ok_ ## OKType ## _ ## ErrType

///
/// Returns a function that casts result to ERR.
///
#define ERR(OKType, ErrType) \
    modc_err_ ## OKType ## _ ## ErrType

///
/// Returns an "untyped" unwrap function.
///
#define INVOKE_UNWRAP(OKType, ErrType) \
    modc_invoke_unwrap_ ## OKType ## _ ## ErrType

///
/// Returns a type of INVOKE_UNWRAP.
///
#define INVOKE_UNWRAP_FN_TYPE(OKType, ErrType) \
    modc_InvokeUnwrap_ ## OKType ## _ ## ErrType

///
/// Represents a kind of RESULT.
///
typedef enum {
    MODC_RESULT_OK,
    MODC_RESULT_ERR,
} ResultKind;

///
/// Declares the struct and functions for designated types.
///
#define DEFINE_RESULT(OKType, ErrType) \
    typedef struct { \
        ResultKind kind; \
        union { \
            OKType* ok; \
            ErrType* err; \
        }; \
    } RESULT_DEREF(OKType, ErrType); \
    RESULT(OKType, ErrType) OK(OKType, ErrType) (OKType* result); \
    RESULT(OKType, ErrType) ERR(OKType, ErrType) (ErrType* result); \
    void* INVOKE_UNWRAP(OKType, ErrType) (modc_Result* result, volatile ErrType** err_obj, jmp_buf* err_jmp_point); \
    typedef void* (*INVOKE_UNWRAP_FN_TYPE(OKType, ErrType)) (modc_Result* result, volatile ErrType** err_obj, jmp_buf* err_jmp_point);

///
/// A result that consists of untyped items.
///
typedef struct {
    ResultKind kind;
    union {
        void* ok;
        void* err;
    };
} modc_Result;

///
/// Implements RESULT for designated types.
///
#define IMPL_RESULT(OKType, ErrType) \
    RESULT(OKType, ErrType) OK(OKType, ErrType) (OKType* result) { \
        RESULT(OKType, ErrType) res = calloc(1, sizeof(RESULT_DEREF(OKType, ErrType))); \
        res->kind = MODC_RESULT_OK; \
        res->ok = result; \
        return res; \
    } \
    RESULT(OKType, ErrType) ERR(OKType, ErrType) (ErrType* result) { \
        RESULT(OKType, ErrType) res = calloc(1, sizeof(RESULT_DEREF(OKType, ErrType))); \
        res->kind = MODC_RESULT_ERR; \
        res->err = result; \
        return res; \
    } \
    void* INVOKE_UNWRAP(OKType, ErrType) (modc_Result* result, volatile ErrType** err_obj, jmp_buf* err_jmp_point) { \
        if (result->kind == MODC_RESULT_OK) \
            return result->ok; \
        *err_obj = (ErrType*)result->err; \
        longjmp(*err_jmp_point, 1); \
        return NULL; \
    }

///
/// Handles early-return from UNWRAP.
///
#define HANDLE_ERR(OKType, ErrType) \
    jmp_buf __modc_err_jmp_point; volatile ErrType* __modc_err_obj; \
    INVOKE_UNWRAP_FN_TYPE(OKType, ErrType) __modc_invoke_unwrap = &INVOKE_UNWRAP(OKType, ErrType); \
    if (setjmp(__modc_err_jmp_point)) \
        return ERR(OKType, ErrType)((ErrType*)__modc_err_obj)

#if defined(TYPEOF)
    ///
    /// Unwraps RESULT. If it holds an error info, exit the function early.
    /// Do not forget to call HANDLE_ERR before using this.
    ///
#   define UNWRAP(result) \
        (TYPEOF(result->ok))(*__modc_invoke_unwrap)((modc_Result*)result, &__modc_err_obj, &__modc_err_jmp_point)
#endif
