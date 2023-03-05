#ifndef LEXER_H
#define LEXER_H

#include "main.h"

enum TokenType get_nextToken(struct Lexer *lexer);
char *my_strndup(const char *s, size_t n);
char *my_strdup(char *s);
void lexer_init(struct Lexer *lexer, char *input);
int lexer_quote(struct Lexer *lexer, int wword, char *input);
int find_basic(struct Lexer *lexer, int wword, char *input);
void lexer_advance(struct Lexer *lexer);

#endif // ! LEXER_H
