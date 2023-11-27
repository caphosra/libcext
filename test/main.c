#include "modc/test/option.h"
#include "modc/test/result.h"

#include <stdlib.h>
#include <string.h>

const char MODC_TEST_FAILED = 1;
const char MODC_TEST_PASSING = 0;

#define RUN_TEST(func) if (!strcmp(test_name, #func)) { func(); return 0; }

int main(int argc, char* argv[]) {
    if (argc < 2) exit(1);
    char* test_name = argv[1];

    RUN_TEST(test_option_some_none);
    RUN_TEST(test_option_unwrap_op);

    RUN_TEST(test_result_ok_err);
    RUN_TEST(test_result_unwrap);

    return 0;
}
