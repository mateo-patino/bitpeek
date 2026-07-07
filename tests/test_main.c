#include <stdio.h>

#include "test_assert.h"
#include "test_lexer.h"
#include "test_ast.h"

#define LINE "------------"

int main(void) {

    int total_tests = 0;
    int total_crashes = 0;
    int total_pass = 0;

    fprintf(stdout, "\n");
    fprintf(stdout, BOLD LINE "LEXER TESTS" LINE ANSI_RESET "\n"); 
    fflush(stdout);

    int total_lex = 0;
    int crashes_lex = 0;
    int pass_lex = run_lexer_tests(&total_lex, &crashes_lex);
    fprintf(stdout, BOLD "Lexer Results: %i total, %i pass, %i fail (%i crash)" ANSI_RESET "\n", total_lex, 
            pass_lex, total_lex - pass_lex, crashes_lex);
    fflush(stdout);
    total_tests += total_lex;
    total_crashes += crashes_lex;
    total_pass += pass_lex;


    fprintf(stdout, "\n");
    fprintf(stdout, BOLD LINE "AST TESTS" LINE ANSI_RESET "\n"); 
    fflush(stdout);

    int total_ast = 0;
    int crashes_ast = 0;
    int pass_ast = run_ast_tests(&total_ast, &crashes_ast);
    fprintf(stdout, BOLD "AST Results: %i total, %i pass, %i fail (%i crash)" ANSI_RESET "\n", total_ast, 
            pass_ast, total_ast - pass_ast, crashes_ast);
    fflush(stdout);
    total_tests += total_ast;
    total_crashes += crashes_ast;
    total_pass += pass_ast;

    fprintf(stdout, "\n");
    fprintf(stdout, BOLD LINE "SUMMARY" LINE ANSI_RESET "\n");
    fprintf(stdout, BOLD "Total tests: %i" ANSI_RESET "\n", total_tests);
    fprintf(stdout, BOLD "Successful: %i" ANSI_RESET "\n", total_pass);
    fprintf(stdout, BOLD "Failed: %i (%i crash)" ANSI_RESET "\n", total_tests - total_pass, total_crashes);
    fprintf(stdout, BOLD ANSI_CYAN "Overall success %.0f%%" ANSI_RESET "\n", 100 * (float)total_pass / total_tests);
    fflush(stdout);

    return 0;
}
