#include "ast.h"
#include "token.h"

#include <stdlib.h>
#include <limits.h>
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
    if (!tokens) {
        return -1;
    }
    token_type type;
    operand_tuple last_op = { CHAR_MAX, CHAR_MAX, -1 };
    char curr_depth = 0;
    for (int i = low; i <= high; i++) {
        type = tokens[i].type;
        if (type == NUMBER) {
            continue;
        }
        else if (type == LPAREN) {
            curr_depth++;
        }
        else if (type == RPAREN) {
            curr_depth--;
        }
        else if (type == OPERAND) {
            operand_t *operand = tokens[i].obj;

            /* Choose op with smallest depth if not tied */
            if (curr_depth < last_op.depth) {
                last_op.depth = curr_depth;
                last_op.precedence = operand->precedence;
                last_op.index = i;
            }
            else if (curr_depth == last_op.depth) {

                /* Choose op with lowest precedence if depth is equal */
                if (operand->precedence < last_op.precedence) {
                    last_op.precedence = operand->precedence;
                    last_op.index = i;
                }
                else if (operand->precedence == last_op.precedence) {
                    /*
                    * If same depth and precedence, choose the rightmost op if the operators
                    * are left-associative and the leftmost op if they are right-associative. 
                    * Note: Two operators with the same precedence MUST have equal associativity, 
                    * so we can check the assoc of either 'operand' or 'last_op' 
                    */
                    if (op_associativity[operand->op] == ASSOC_LEFT) {
                        if (i > last_op.index) {
                            last_op.index = i;
                        }
                    }
                    else if (op_associativity[operand->op] == ASSOC_RIGHT) {
                        if (i < last_op.index) {
                            last_op.index = i;
                        }
                    }
                }
            }
        }
    }
    return last_op.index;
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


