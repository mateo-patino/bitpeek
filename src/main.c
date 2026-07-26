/*
* Entry point for bitpeek
*/
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <inttypes.h>
#include <errno.h>

#include "token.h"
#include "lexer.h"
#include "ast.h"
#include "printer.h"

#define LEADING_OUTPUT_SPACE "    "
#define ANSI_BOLD "\x1b[1m"
#define ANSI_RESET "\x1b[0m"

int is_valid_grouping(char *grp_by_str) {
    if (!grp_by_str || *grp_by_str == '\0') {
        return -1;
    }
    char *end;
    errno = 0;
    long res = strtol(grp_by_str, &end, 10);

    if (*end != '\0' || end == grp_by_str) {
        return -1;
    }
    else if (errno != 0) {
        return -1;
    }

    return res >= 0 ? (int)res : -1;
}


void bitpeek_help(void) {
    fprintf(stdout,
        ANSI_BOLD "Usage:" ANSI_RESET " bitpeek " ANSI_BOLD "[OPTIONS] EXPRESSION" ANSI_RESET "\n"
        "\n"
        "Evaluate an unsigned integer expression and print the result in binary,\n"
        "octal, decimal, and hexadecimal.\n"
        "\n"
        ANSI_BOLD "Options:" ANSI_RESET "\n"
        "  " ANSI_BOLD "-b N" ANSI_RESET "    Group binary digits in sets of N.\n"
        "  " ANSI_BOLD "-o N" ANSI_RESET "    Group octal digits in sets of N.\n"
        "  " ANSI_BOLD "-x N" ANSI_RESET "    Group hexadecimal digits in sets of N.\n"
        "  " ANSI_BOLD "-c" ANSI_RESET "      Print hexadecimal digits in uppercase.\n"
        "  " ANSI_BOLD "-h" ANSI_RESET "      Show this help message and exit.\n"
        "\n"
        ANSI_BOLD "N" ANSI_RESET " must be a non-negative integer. A grouping value of 0 disables grouping.\n"
        "\n"
        ANSI_BOLD "Expressions:" ANSI_RESET "\n"
        "  " ANSI_BOLD "Surround every number, operator, and parenthesis with whitespace." ANSI_RESET "\n"
        "  Numbers may be decimal (42), binary (0b101010), octal (052), or\n"
        "  hexadecimal (0x2a). Quote the full expression when invoking bitpeek to\n"
        "  avoid escaping shell characters.\n"
        "\n"
        ANSI_BOLD "Operators:" ANSI_RESET "\n"
        "  " ANSI_BOLD "NOT" ANSI_RESET ":           not, ~, bitnot\n"
        "  " ANSI_BOLD "Multiply" ANSI_RESET ":      mul, times, *\n"
        "  " ANSI_BOLD "Divide" ANSI_RESET ":        div, over, /\n"
        "  " ANSI_BOLD "Add" ANSI_RESET ":           add, plus, +\n"
        "  " ANSI_BOLD "Subtract" ANSI_RESET ":      sub, minus, -\n"
        "  " ANSI_BOLD "Left shift" ANSI_RESET ":    lshift, <<, leftshift\n"
        "  " ANSI_BOLD "Right shift" ANSI_RESET ":   rshift, >>, rightshift\n"
        "  " ANSI_BOLD "AND" ANSI_RESET ":           and, &, bitand\n"
        "  " ANSI_BOLD "XOR" ANSI_RESET ":           xor, ^, bitxor\n"
        "  " ANSI_BOLD "OR" ANSI_RESET ":            or, |, bitor\n"
        "\n"
        ANSI_BOLD "Examples:" ANSI_RESET "\n"
        "  bitpeek \"10 + 20\"\n"
        "  bitpeek -b 4 -x 2 \"( 0b1010 << 1 ) & 0xff\"\n"
    );
}

/* 
* Prints 'n' token_t structs from an array. Used for debugging and development only.
*/
void show_token_array(const token_t *tok, int count) {
    fprintf(stdout, "[");
    for (int i = 0; i < count - 1; i++) {
        print_token(tok + i, false);
        fprintf(stdout, ", ");
    }
    print_token(tok + count - 1, false);
    fprintf(stdout, "]\n");
}


int main(int argc, char** argv) {

    int group_bin_by = 0, group_oct_by = 0, group_hex_by = 0; 
    bool accept_grouping = true;
    bool print_caps = false; 

    opterr = 0;
    int c = 0;
    while ((c = getopt(argc, argv, ":b:o:x:ch")) != -1) {
        switch(c) {
            case 'b':
                group_bin_by = is_valid_grouping(optarg); 
                accept_grouping = group_bin_by == -1 || !accept_grouping ? false : true;
                break;
            case 'o':
                group_oct_by = is_valid_grouping(optarg);
                accept_grouping = group_oct_by == -1 || !accept_grouping ? false : true;
                break;
            case 'x':
                group_hex_by = is_valid_grouping(optarg);
                accept_grouping = group_hex_by == -1 || !accept_grouping ? false : true;
                break;
            case 'c':
                print_caps = true;
                break;
            case 'h':
                bitpeek_help();
                return EXIT_SUCCESS;
            case ':':
                fprintf(stderr, "Error: Missing group-by argument.\n");
                break;
            case '?':
                fprintf(stderr, "Error: Unknown option '-%c' received.\n", optopt);
                return EXIT_FAILURE;
        }
    }

    if (!accept_grouping) {
        fprintf(stderr, "Error: Invalid group-by argument.\n");
        return EXIT_FAILURE;
    }

    size_t token_count = 0;
    bool tokenize_argv = true;
    if (optind == argc) {
        fprintf(stderr, "Error: missing expression.\n");
        return EXIT_FAILURE;
    }
    else if (argc - optind >= 2) {
        token_count = argc - optind;
        /* tokenize_argv remains true in this case */
    }
    /* One non-option argument remaining is interpreted as an expression enclosed by quotes */
    else if (argc - optind == 1) {
        token_count = count_tokens(argv[optind]);
        tokenize_argv = false;
    }

    /* Produce array of tokens */
    token_t tokens[token_count];
    tokens_status tok_status;
    char *invalid = NULL;

    /* Valid ONLY when tokenizing from string instead of argv. Declared here to extend 'string_buf' 
     * lifetime until print_token_error attempts to access 'invalid', which would point to 'string_buf' */
    size_t string_len = strlen(argv[optind]); 
    char string_buf[string_len + 1]; 

    if (tokenize_argv) {
        argv += optind;
        tok_status = create_tokens_from_argv(argv, tokens, &invalid);
    }
    else {
        strncpy(string_buf, argv[optind], string_len + 1);
        string_buf[string_len] = '\0';
        tok_status = create_tokens_from_string(string_buf, tokens, &invalid);
    }

    if (tok_status != TOKENS_OK) {
        print_token_error(tok_status, invalid);
        free_tokens_invalid(tokens); 
        return EXIT_FAILURE;
    }

    /* Ensure the tokens form a valid mathematical expression that can be parsed by the AST module */
    if ((tok_status = validate_tokens_semantic(tokens, token_count)) != TOKENS_OK) {
        /* TOKENS_EXPECTED_OPERAND error message is printed inside validators to inform user of which operator
        * is missing an operand, hence we skip printing out here if that error is returned */
        if (tok_status != TOKENS_EXPECTED_OPERAND) {
            print_token_error(tok_status, NULL);
        }
        free_tokens_count(tokens, token_count);
        return EXIT_FAILURE;
    }

    /* Exit status for main */
    int retval = EXIT_SUCCESS;

    /* Parse the token array to build an Abstract Syntax Tree (AST) */
    ast_status ast_create_status = AST_OK;
    AST ast;
    ast.root = create_ast_from_tokens(tokens, token_count, &ast_create_status); 
    if (!ast.root || ast_create_status != AST_OK) {
        print_ast_error(ast_create_status, NULL);
        retval = EXIT_FAILURE;
        goto FREE_AND_EXIT;
    }

    /* Evaluate the AST */
    ast_status ast_eval_status = AST_OK;
    value_t out = evaluate_ast(&ast, &ast_eval_status);
    if (ast_eval_status != AST_OK) {
        print_ast_error(ast_eval_status, NULL);
        retval = EXIT_FAILURE;
        goto FREE_AND_EXIT;
    }

    /* Pretty print */
    fprintf(stdout, LEADING_OUTPUT_SPACE "Base 2:        ");
    if (!group_bin_by) {
        raw_print_binary(stdout, out, true);
    }
    else {
        pretty_print_binary(stdout, out, group_bin_by, true);
    }

    fprintf(stdout, LEADING_OUTPUT_SPACE "Base 8:        ");
    if (!group_oct_by) {
        raw_print_octal(stdout, out, true);
    }
    else {
        pretty_print_octal(stdout, out, group_oct_by, true);
    }

    fprintf(stdout, LEADING_OUTPUT_SPACE "Base 10:       ");
    pretty_print_decimal(stdout, out, true);
    
    fprintf(stdout, LEADING_OUTPUT_SPACE "Base 16:       ");
    if (!group_hex_by) {
        raw_print_hexadecimal(stdout, out, print_caps, true);
    }
    else {
        pretty_print_hexadecimal(stdout, out, group_hex_by, print_caps, true);
    }

FREE_AND_EXIT:
    free_ast(&ast);
    free_tokens_count(tokens, token_count);
    return retval;
}
