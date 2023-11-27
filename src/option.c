#include "cpoly/option.h"

#include <setjmp.h>

OptionBase* __cpoly_some(void* some) {
    OptionBase* option = calloc(1, sizeof(OptionBase));
    option->kind = OPTION_SOME;
    option->some = some;
    return option;
}

void* __cpoly_unwrap_option(OptionBase* option, jmp_buf* none_jmp_point) {
    if (option->kind == OPTION_SOME)
        return option->some;
    longjmp(*none_jmp_point, 1);
    return NULL;
}
