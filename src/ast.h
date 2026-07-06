#ifndef AST_H
#define AST_H

#include "token.h"


/* AST node */
typedef struct ASTNode {
    const token_t *token;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;


/* 
* Initializes a parametrized ASTNode object. It returns the address of the new node, 
* which is allocated in HEAP memory. 
*/
ASTNode *init_ast_node(const token_t *tok, ASTNode *left, ASTNode *right);


/* AST object */
typedef struct {
    ASTNode *root;
} AST;


/*
* Builds an Abstract Syntax Tree (AST) given a tokens array 'tokens' and the number
* of tokens in the array 'tc' (token count).
*
* Returns a pointer to an ASTNode representing the root of the tree.
*/
ASTNode *create_ast_from_tokens(const token_t *tokens, size_t tc);


/*
* Called by create_ast_from_tokens to perform the recursive routine and build the AST.
*/
ASTNode *create_ast_helper(const token_t *tokens, int low, int high);

typedef struct {
    char depth;
    char precedence;
    int index;
} last_op_tuple;

/*
* Searches for the operator token that represents the last operation to be evaluated in
* the range [low, high] (inclusive).
*
* It returns the index of this operator token upon success and -1 if no token is found.
*
* It compares operators by recording 3 values about each one: depth, precedence, and index.
* Every operator token has a unique tuple of these 3 values.
*/
int find_last_operation(const token_t *tokens, int low, int high);


/*
* Returns the index of a single token of number type found in the range [low, high].
*
* It returns -1 if none were found or if more than one number token exists in the range (this signifies 
* that the AST building algorithm is wrong).
*/
int find_only_number(const token_t *tokens, int low, int high);


/* 
* Returns true if an array of tokens has at least one operation token in the range 
* contaiing the indices [low, high].
*/
bool has_any_operations(const token_t *tokens, int low, int high);


#endif
