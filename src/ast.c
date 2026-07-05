#include "ast.h"

#include <stdlib.h>
#include <stdio.h>


ASTNode *init_ast_node(const token_t *tok, ASTNode *left, ASTNode *right) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) {
        return NULL;
    }
    node->token = tok;
    node->left = left;
    node->right = right;

    return node;
}


ASTNode *create_ast_from_tokens(const token_t *tokens, size_t tc) {
    if (!tokens) {
        return NULL;
    }
    return create_ast_helper(tokens, 0, tc - 1);
}


ASTNode *create_ast_helper(const token_t *tokens, int low, int high) {
    /* Only one token in the range */
    if (low == high) {
        return init_ast_node(tokens + low, NULL, NULL);
    }
    /* Multiple tokens in the range but no more operations (only parens and number remain, so extract the number) */
    else if (!has_any_operations(tokens, low, high)) {
        int on_index = find_only_number(tokens, low, high);

        /* DEBUG PURPOSES. TODO: remove after this invariant has been proven through testing */
        if (on_index == -1) { 
            fprintf(stderr, "\x1b[31m" "Not having any operations does not guarantee that EXACTLY ONE number token exists.\n" "\x1b[0m"); 
            return NULL;
        }
        /* DEBUG PURPOSES */

        return init_ast_node(tokens + on_index, NULL, NULL);
    }

    int lo_index = find_last_operation(tokens, low, high);
    ASTNode *left = create_ast_helper(tokens, low, lo_index - 1);
    ASTNode *right = create_ast_helper(tokens, lo_index + 1, high);

    return init_ast_node(tokens + lo_index, left, right);
}


int find_last_operation(const token_t *tokens, int low, int high) {
    (void)high;
    (void)low;
    (void)tokens;
    return 1;
}



int find_only_number(const token_t *tokens, int low, int high) {
    if (!tokens) {
        return NULL;
    }
    int index = -1;
    for (int i = low; i <= high; i++) {
        if (tokens[i].type == NUMBER) {
            if (index != -1) {
                return -1;
            }
            index = i;
        }
    }
    return index;
}


bool has_any_operations(const token_t *tokens, int low, int high) {
    if (!tokens) {
        return NULL;
    }
    for (int i = low; i <= high; i++) {
        if (tokens[i].type == OPERAND) {
            return true;
        }
    }
    return false;
}


