/* Miscellaneous helpers used in unit and integration tests */
#include <stdio.h>
#include <inttypes.h>
#include "test_assert.h"
#include "token.h"


void assert_true_failed(const char *file_name, int line, const char *func, const char *expr) {
    fprintf(stderr, BOLD "%s " ANSI_RED "FAILED" ANSI_RESET "\n", func);
    fprintf(stderr, "   Assertion failed: %s\n", expr);
    fprintf(stderr, "   at %s:%i\n", file_name, line);
    fprintf(stderr, "   in %s\n", func);
}


void assert_eq_int_failed(int exp, int rec, const char *file_name, int line, const char* func) {
    fprintf(stderr, BOLD "%s " ANSI_RED "FAILED" ANSI_RESET "\n", func);
    fprintf(stderr, "   Expected: %i Received: " BOLD "%i" ANSI_RESET "\n",  exp, rec);
    fprintf(stderr, "   at %s:%i\n", file_name, line);
    fprintf(stderr, "   in %s\n", func);
}


void assert_eq_number_t_failed(const number_t *exp, const number_t *rec, const char *file_name, int line, const char *func) {
    fprintf(stderr, BOLD "%s " ANSI_RED "FAILED" ANSI_RESET "\n", func);
    fprintf(stderr, "   Expected: number_t(val=%" PRIu64 ", base=%i)\n", exp->value, exp->base);
    fprintf(stderr, "   Received:" BOLD "number_t(val=%" PRIu64 ", base=%i)" ANSI_RESET "\n", rec->value, rec->base);
    fprintf(stderr, "   at %s:%i\n", file_name, line);
    fprintf(stderr, "   in %s\n", func);
}


void assert_eq_value_t_failed(value_t exp, value_t rec, const char *file_name, int line, const char *func) {
    fprintf(stderr, BOLD "%s " ANSI_RED "FAILED" ANSI_RESET "\n", func);
    fprintf(stderr,     "Expected: %" PRIu64 " Received: " BOLD "%" PRIu64 ANSI_RESET "\n",  exp, rec);
    fprintf(stderr, "   at %s:%i\n", file_name, line);
    fprintf(stderr, "   in %s\n", func);
}
