/*
* Entry point for pcalc.
*/
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "token.h"
#include "lexer.h"


/*
 * Returns the number of whitespace ' ' sequences in a string and -1 if the string is NULL.
 * It skips all leading and trailing whitespace.
 */
int count_whitespaces(const char *str) {
    if (!str) {
        return -1;
    }
    int count = 0;
    bool seen_word = false;
    for (size_t i = 0, len = strlen(str); i < len; i++) {
        if (isspace((unsigned char)str[i])) {
            if (seen_word) {
                count++;
                seen_word = false;
            }
        }
        else { seen_word = true; }
    }

    if (!seen_word && count > 1) { count--; }
    return count;


    if (!str) {
        return -1;
    }


}

/* 
TODO: Usage and help function.
*/
void pcalc_help() {
    /* TODO */
    return;
}



int main(int argc, char** argv) {

    bool b_flag = false, o_flag = false, d_flag = false, x_flag = false, all_bases = true;    
    /*
    * To control the base of the output, use the following options:
    * -b: binary
    * -o: octal
    * -d: decimal
    * -x: hexadecimal
    * -h: help
    */

    opterr = 0;
    char c = 0;
    while ((c = getopt(argc, argv, "+bodxh")) != -1) {
        switch(c) {
            case 'b':
                b_flag = true;
                break;
            case 'o':
                o_flag = true;
                break;
            case 'd':
                d_flag = true;
                break;
            case 'x':
                x_flag = true;
                break;
            case 'h':
                pcalc_help();
                return EXIT_SUCCESS;
            case '?':
                fprintf(stderr, "Error: Unknown option '-%c' received.\n", optopt);
                return EXIT_FAILURE;
        }
    }

    if (b_flag + o_flag + d_flag + x_flag > 1) {
        fprintf(stderr, "Error: too many options received.\n");
        return EXIT_FAILURE;
    }
    else if (b_flag + o_flag + d_flag + x_flag == 1) {
        all_bases = false;
    }

    size_t token_count = 0;
    bool tokenize_argv;
    if (optind == argc) {
        fprintf(stderr, "Error: missing expression.\n");
        return EXIT_FAILURE;
    }
    else if (argc - optind >= 2) {
        token_count = argc - optind;
        tokenize_argv = true;
    }
    /* One non-option argument remaining is interpreted as an expression enclosed by quotes */
    else if (argc - optind == 1) {
        token_count = count_whitespaces(argv[optind]) + 1;
        tokenize_argv = false;
    }

    /* Produce array of tokens */
    token_t tokens[token_count];
    tokens_status tok_status;
    char *invalid = NULL;

    if (tokenize_argv) {
        argv += optind;
        tok_status = create_tokens_from_argv(argv, tokens, &invalid);
    }
    else {
        size_t len = strlen(argv[optind]); 
        char string[len + 1]; 
        strncpy(string, argv[optind], len + 1);
        string[len] = '\0';
        tok_status = create_tokens_from_string(string, tokens, &invalid);
    }

    if (tok_status != TOKENS_OK) {
        print_token_error(tok_status);
        if (invalid) {
            fprintf(stderr, "'%s'", invalid);
        }
        return EXIT_FAILURE;
    }

    
    /*
    * TODO:
    * 1) Take argv and produce a token array
    * 2) Build an AST with the token array
    * 3) Perform semantic analysis (i.e. does the math make sense, type checking and errors)
    * 4) Traverse the AST to evaluate
    * 5) Output the result
    */

    return EXIT_SUCCESS;
}
