#pragma once

#include "modc/type.h"

#include <stdlib.h>
#include <setjmp.h>

///
/// Represents a kind of OPTION.
///
typedef enum {
    MODC_OPTION_NONE = 0,
    MODC_OPTION_SOME,
} OptionKind;

///
/// An option that contains none or just one item.
///
typedef struct {
    OptionKind kind;
    void* some;
} OptionBase;

OptionBase* __modc_some(void* some);
void* __modc_unwrap_option(OptionBase* option, jmp_buf* none_jmp_point);

///
/// Gets the dereferenced type of OPTION.
/// In most cases, it would be better to use OPTION.
///
#define OPTION_DEREF(SomeType) \
    struct { \
        OptionKind kind; \
        SomeType* some; \
    }

///
/// Gets the type of OPTION.
///
#define OPTION(SomeType) OPTION_DEREF(SomeType)*

///
/// Casts an item to an OPTION.
///
#define SOME(item, type) \
    (type)ASSERT_TYPE(item, ((type)NULL)->some, __modc_some)((void*)item)

///
/// Returns NONE.
///
#define NONE(type) ((type) calloc(1, sizeof(OptionBase)))

///
/// Handles early-return from UNWRAP_OP.
///
#define HANDLE_NONE(type) \
    jmp_buf __modc_none_jmp_point; \
    if (setjmp(__modc_none_jmp_point)) \
        return NONE(type)

///
/// Unwraps OPTION. If it holds nothing, exit the function early.
/// Do not forget to call HANDLE_NONE before using this.
///
#define UNWRAP_OP(option) \
    (TYPEOF(option->some)) ((OptionBase*)option, &__modc_none_jmp_point)
