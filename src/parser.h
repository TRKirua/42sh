#ifndef PARSER_H
#define PARSER_H

#include "main.h"

void skip_newline(struct Lexer *lexer);
struct Ast *parse(struct Lexer *lexer);
struct Table *init_table(void);
struct Cmd_Node *parse_CommandList(struct Lexer *lexer, char **var);
int parse_CommandList_sub(struct Lexer *lexer, struct Table *table,
                          struct Cmd_Node *Cmd);
void parse_CommandList_sub_sub(struct Lexer *lexer, struct Table *table,
                               struct Cmd_Node *Cmd);
struct If_Node *parse_IfCommand(struct Lexer *lexer, int need_fi);
struct If_Node *parse_IfCommand_sub(struct Lexer *lexer, struct If_Node *list);
struct If_Node *parse_IfCommand_sub_sub(struct Lexer *lexer,
                                        struct If_Node *list);
void exclamation_change(struct Lexer *lexer, struct Cmd_Node *cmd);

#endif // ! PARSER_H
