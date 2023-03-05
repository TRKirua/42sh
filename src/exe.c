#include "exe.h"

char *readCommandLine(void)
{
    char *line = malloc(512);
    if (!line)
    {
        perror("Malloc failed");
        exit(1);
    }

    if (!fgets(line, 512, stdin))
    {
        free(line);
        return NULL;
    }

    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';

    return line;
}

int execute_Ast(struct Ast *ast)
{
    int res = 0;

    if (ast->type == COMMAND_LIST)
        res = cmd_result(ast);

    else if (ast->type == IF)
        res = if_result(ast);

    else if (ast->type == WHILE)
        res = while_result(ast);

    else if (ast->type == FOR)
        res = for_result(ast);

    return res;
}

int executeCommand(char *line)
{
    struct Lexer *lexer = my_calloc(1, sizeof(struct Lexer));
    lexer_init(lexer, line);

    if (get_nextToken(lexer) == TOKEN_EOF)
        return 0;

    struct Ast *ast = NULL;
    int exe = 2;

    while ((ast = parse(lexer)) != NULL)
        exe = execute_Ast(ast);

    my_free();
    return exe;
}
