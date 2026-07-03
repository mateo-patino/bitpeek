#include "test_lexer.h"
#include "test_assert.h"
#include "token.h"
#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))


static const test_case_t lexer_tests[] = {
    TEST(test_valid_arithmetic_op_tokenizer),
    TEST(test_invalid_arithmetic_op_tokenizer),
    TEST(test_valid_decimal_tokenizer),
    TEST(test_invalid_decimal_tokenizer)
};


/* Runner for the entire module */
int run_lexer_tests(int *total_ran, int *crashes) {
    size_t test_count = ARRAY_LEN(lexer_tests);
    int pass = 0;
    int signaled = 0;
    pid_t test_pid;

    for (size_t i = 0; i < test_count; i++) {

        if ((test_pid = fork()) < 0) {
            fprintf(stderr, "fork() failed. %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        /* Child process running the test */
        else if (test_pid == 0) {
            if (lexer_tests[i].func()) {
                _exit(EXIT_SUCCESS);
            }
            _exit(EXIT_FAILURE);
        }
        /* Runner process (parent) */
        int status;
        pid_t terminated_pid = waitpid(test_pid, &status, 0);
        while (terminated_pid == -1 && errno == EINTR) {
            terminated_pid = waitpid(test_pid, &status, 0);
        }

        if (terminated_pid == -1) {
            fprintf(stderr, "waitpid() failed. %s\n", strerror(errno));
            fprintf(stderr, "Could not read result of %s\n", lexer_tests[i].name);
            continue;
        }
        
        /* Check for normal exit */
        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            if (exit_code == EXIT_SUCCESS) {
                fprintf(stdout, "%s " ANSI_GREEN "PASS" ANSI_RESET "\n", lexer_tests[i].name);
                fflush(stdout);
                pass++;
            }
            continue;
        }
        /* Check for termination by a signal */
        else if (WIFSIGNALED(status)) {
            fprintf(stderr, "%s " ANSI_RED "CRASHED" ANSI_RESET "\n", lexer_tests[i].name); 
            signaled++;
        }
    }

    if (total_ran) { *total_ran = (int)test_count; }
    if (crashes) { *crashes = signaled; }
    return pass;
}


bool test_valid_arithmetic_op_tokenizer(void) {
    operation_type op;

    ASSERT_TRUE(is_operation("add", &op));
    ASSERT_TRUE(op == ADD);
    ASSERT_TRUE(is_operation("plus", &op));
    ASSERT_TRUE(op == ADD);

    ASSERT_TRUE(is_operation("sub", &op));
    ASSERT_TRUE(op == SUB);
    ASSERT_TRUE(is_operation("minus", &op));
    ASSERT_TRUE(op == SUB);

    ASSERT_TRUE(is_operation("mul", &op));
    ASSERT_TRUE(op == MUL);
    ASSERT_TRUE(is_operation("times", &op));
    ASSERT_TRUE(op == MUL);
    
    ASSERT_TRUE(is_operation("div", &op));
    ASSERT_TRUE(op == DIV);
    ASSERT_TRUE(is_operation("over", &op));
    ASSERT_TRUE(op == DIV);

    return true;
}


bool test_invalid_arithmetic_op_tokenizer(void) {
    ASSERT_TRUE(!is_operation("", NULL));
    ASSERT_TRUE(!is_operation(" ", NULL));
    ASSERT_TRUE(!is_operation("\n", NULL));
    ASSERT_TRUE(!is_operation("\\", NULL));
    ASSERT_TRUE(!is_operation("dog", NULL));
    ASSERT_TRUE(!is_operation("0x10", NULL));
    ASSERT_TRUE(!is_operation("adds", NULL));
    ASSERT_TRUE(!is_operation(NULL, NULL));

    return true;
}


bool test_valid_decimal_tokenizer(void) {
     int base;
     value_t val;

     ASSERT_TRUE(is_number("10", &base, &val));
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)10, val);
     
     ASSERT_TRUE(is_number("  10  ", &base, &val));
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)10, val);

     ASSERT_TRUE(is_number("123456789", &base, &val));
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)123456789, val);

     ASSERT_TRUE(is_number("0", &base, &val));
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)0, val);

     ASSERT_TRUE(is_number("1", &base, &val));
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)1, val);

     ASSERT_TRUE(is_number("123456789", &base, &val));
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)123456789, val);

     errno = 0;
     ASSERT_TRUE(is_number("18446744073709551615", &base, &val));
     ASSERT_TRUE(errno == 0);
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)18446744073709551615ULL, val);
 
     return true;
}


bool test_invalid_decimal_tokenizer(void) {
    ASSERT_TRUE(!is_number("-10", NULL, NULL));
    ASSERT_TRUE(!is_number("10-", NULL, NULL));
    ASSERT_TRUE(!is_number("10 -", NULL, NULL));
    ASSERT_TRUE(!is_number("string", NULL, NULL));
    ASSERT_TRUE(!is_number("", NULL, NULL));
    ASSERT_TRUE(!is_number("-123456789", NULL, NULL));
    ASSERT_TRUE(!is_number("18446744073709551616", NULL, NULL));
    ASSERT_TRUE(errno == ERANGE);
    ASSERT_TRUE(!is_number("-18446744073709551614", NULL, NULL));
    ASSERT_TRUE(!is_number(NULL, NULL, NULL));

    return true;
}

