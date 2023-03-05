#ifndef PARSER_TWO_H
#define PARSER_TWO_H

#include "main.h"

int cptargs(struct Lexer *lexer, char **var, struct Cmd_Node *cmd);
struct For_Node *parse_ForCommand_sub(struct Lexer *lexer,
                                      struct For_Node *list, int no,
                                      struct Cmd_Node *cmd);
struct For_Node *parse_ForCommand(struct Lexer *lexer);
void compare_for(int *last, char **token, struct Cmd_Node *list, int *incr);
int verify_and_or(struct Lexer *lexer, struct Cmd_Node *cmd);
void verify_or(struct Lexer *lexer, struct Cmd_Node *cmd, int *res);
struct While_Node *parse_WhileCommand(struct Lexer *lexer);

#endif // ! PARSER_TWO_H
