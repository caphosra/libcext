#include "modc/option.h"

#include <setjmp.h>

OptionBase* __modc_some(void* some) {
    OptionBase* option = calloc(1, sizeof(OptionBase));
    option->kind = MODC_OPTION_SOME;
    option->some = some;
    return option;
}

void* __modc_unwrap_option(OptionBase* option, jmp_buf* none_jmp_point) {
    if (option->kind == MODC_OPTION_SOME)
        return option->some;
    longjmp(*none_jmp_point, 1);
    return NULL;
}
