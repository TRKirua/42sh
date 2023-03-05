#ifndef ASTRUCT_H
#define ASTRUCT_H

#define MAX_TOKENS 50

#include <stdlib.h>
#include <string.h>

enum node_type
{
    COMMAND_LIST,
    IF,
    WHILE,
    FOR,
    UNKNOWN,
};

struct Ast
{
    enum node_type type;
    int then_true;
};

struct Cmd_Node
{
    struct Ast node;
    char ***words;
    int NbrCmd;
    int exclamation;
    int and_or;
    int pipe;
    char **variable;
};

struct If_Node
{
    struct Ast node;
    struct Ast *if_condition; // condition du if
    struct Ast *then_condition;
    struct Ast *elif_condition;
    struct Ast *else_condition;
};

struct While_Node
{
    struct Ast node;
    struct Ast *do_condition;
    struct Ast *done_condition;
};

struct For_Node
{
    struct Ast node;
    struct Ast *do_condition;
    struct Ast *done_condition;
    int cpt_iter;
};

struct Table
{
    int i;
    int j;
    int semicolon;
    int stop;
    int true_false_;
    int incr;
    int last;
};

enum TokenType
{
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELIF,
    TOKEN_ELSE,
    TOKEN_FI,
    TOKEN_SEMICOLON,
    TOKEN_NEWLINE,
    TOKEN_APOSTROPHE,
    TOKEN_WORD,
    TOKEN_EOF,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_REDIR,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_WHILE,
    TOKEN_DO,
    TOKEN_DONE,
    TOKEN_FOR,
    TOKEN_UNTIL,
    TOKEN_PIPE,
    TOKEN_AMPER,
    TOKEN_NOT,
    TOKEN_UNKNOWN,
    TOKEN_REDIR2,
};

struct Token
{
    enum TokenType type;
    char *value;
};

struct Lexer
{
    char *input;
    int index;
    struct Token current_token;
};

#endif // ! ASTRUCT_H
