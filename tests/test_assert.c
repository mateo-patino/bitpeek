/* Miscellaneous helpers used in unit and integration tests */
#include "test_assert.h"
#include <stdio.h>

void assert_true_failed(const char *file_name, int line, const char *func, const char *expr) {
    fprintf(stderr, "'%s' FAILED\n", func);
    fprintf(stderr, "'%s' in line %i\n", file_name, line);
    fprintf(stderr, "Expected '%s' to be true, got false\n", expr);
}
