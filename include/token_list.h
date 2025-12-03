#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

typedef struct TokenNode {
    int key;
    const char *token;
    struct TokenNode *next;
} TokenNode;

void init_token_list(void);

void cleanup_token_list(void);

const char *token_name(int key);

#endif /* TOKEN_LIST_H */
