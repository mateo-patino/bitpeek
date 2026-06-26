/*
* Entry point for pcalc.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

#include "token.h"
#include "lexer.h"

int main(int argc, char** argv) {
bool b_flag = false, o_flag = false, d_flag = false, h_flag = false, all_bases = true;    
    /*
    * To control the base of the output, use the following options:
    * -b: binary
    * -o: octal
    * -d: decimal
    * -h: hexadecimal
    */

    opterr = 0;
    char c = 0;
    while ((c = getopt(argc, argv, "+bodh")) != -1) {
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
            case 'h':
                h_flag = true;
                break;
            case '?':
                fprintf(stderr, "Error: Unknown option '-%c' received.\n", optopt);
                return EXIT_FAILURE;
        }
    }

    if (b_flag + o_flag + d_flag + h_flag > 1) {
        fprintf(stderr, "Error: too many options received.\n");
        return EXIT_FAILURE;
    }
    else if (b_flag + o_flag + d_flag + h_flag == 1) {
        all_bases = false;
    }

    if (optind == argc) {
        fprintf(stderr, "Error: invalid expression.\n");
        return EXIT_FAILURE;
    } 

    /* Produce array of tokens from argv */
    argv += optind;
    token_t tokens[argc - optind];
    char *invalid = NULL;
    tokens_status tok_status = create_tokens_from_argv(argv, tokens, &invalid);

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
