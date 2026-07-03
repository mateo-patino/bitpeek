#include <stdio.h>

#include "test_assert.h"
#include "test_lexer.h"

#define LINE "------------"

int main(void) {

    fprintf(stdout, BOLD LINE "LEXER TESTS" LINE ANSI_RESET "\n"); 
    int total_lexer = 0;
    int pass_lexer = run_lexer_tests(&total_lexer);
    fprintf(stdout, "Lexer Results: %i total, %i pass, %i fail\n", total_lexer, 
            pass_lexer, total_lexer - pass_lexer);

    return 0;
}
