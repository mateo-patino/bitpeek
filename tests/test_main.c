#include <stdio.h>

#include "test_assert.h"
#include "test_lexer.h"

#define LINE "------------"

int main(void) {

    fprintf(stdout, "\n");
    fflush(stdout);
    fprintf(stdout, BOLD LINE "LEXER TESTS" LINE ANSI_RESET "\n"); 
    fflush(stdout);

    int total_lex = 0;
    int crashes_lex = 0;
    int pass_lex = run_lexer_tests(&total_lex, &crashes_lex);
    fprintf(stdout, BOLD "Lexer Results: %i total, %i pass, %i fail (%i crash)" ANSI_RESET "\n", total_lex, 
            pass_lex, total_lex - pass_lex, crashes_lex);
    fflush(stdout);

    return 0;
}
