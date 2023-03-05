#include "parser_two.h"

int cpt = 0;

void compare_for(int *last, char **token, struct Cmd_Node *list, int *incr)
{
    if (*token[0] == '$' && list->variable)
    {
        int m = 0;
        int k = 1;

        char *var = *token;

        while (var[k] == list->variable[0][m] && list->variable[0][m] && var[k])
        {
            k++;
            m++;
        }

        if (!var[k] && !list->variable[0][m])
        {
            if (*incr <= cpt)
            {
                *token = list->variable[*incr];
                *incr += 1;

                if (cpt != 1 && *incr - 1 != cpt)
                {
                    *last = strlen(*token) + 7;
                }
            }
        }

        else
            *token = "";
    }

    else if (strcmp("echo", *token))
    {
        if (*incr <= cpt)
        {
            *incr += 1;

            if (cpt != 1 && *incr - 1 != cpt)
            {
                *last = strlen(*token) + 7;
            }
        }
    }
}

int cptargs(struct Lexer *lexer, char **var, struct Cmd_Node *cmd)
{
    lexer_advance(lexer);

    cpt = 0;

    int i = 1;

    while (lexer->current_token.type != TOKEN_SEMICOLON)
    {
        cpt++;
        var[i] = lexer->current_token.value;
        i++;
        cmd->variable[i] =
            my_calloc(strlen(lexer->current_token.value) + 1, sizeof(char));
        strcpy(cmd->variable[i], lexer->current_token.value + 1);
        enum TokenType next = get_nextToken(lexer);

        if (next == TOKEN_DONE)
            break;

        lexer_advance(lexer);

        if (lexer->current_token.type == TOKEN_DO)
            return -1;
    }

    lexer_advance(lexer);

    return cpt;
}

struct For_Node *parse_ForCommand(struct Lexer *lexer)
{
    struct For_Node *list = my_calloc(1, sizeof(struct For_Node));
    list->node.type = FOR;

    skip_newline(lexer);
    lexer_advance(lexer);

    struct Cmd_Node *cmd = my_calloc(1, sizeof(struct Cmd_Node));
    cmd->variable = my_calloc(100, sizeof(char *));
    cmd->variable[0] =
        my_calloc(strlen(lexer->current_token.value) + 1, sizeof(char));
    strcpy(cmd->variable[0], lexer->current_token.value);

    int no = 0;
    lexer_advance(lexer);

    if (lexer->current_token.type != TOKEN_SEMICOLON)
    {
        if (strcmp(lexer->current_token.value, "in"))
        {
            lexer_advance(lexer);
            fprintf(stderr, "42sh: syntax error near unexpected token `%s'\n",
                    lexer->current_token.value);
            return NULL;
        }

        else
        {
            list->cpt_iter = cptargs(lexer, cmd->variable, cmd);

            if (list->cpt_iter == -1)
                return NULL;
        }
    }

    else
    {
        list->node.type = UNKNOWN;
        no = 1;
        lexer_advance(lexer);
    }

    if (lexer->current_token.type != TOKEN_DO)
    {
        fprintf(stderr, "42sh: syntax error near unexpected token `do'\n");
        return NULL;
    }

    else
    {
        list = parse_ForCommand_sub(lexer, list, no, cmd);
    }

    return list;
}

struct For_Node *parse_ForCommand_sub(struct Lexer *lexer,
                                      struct For_Node *list, int no,
                                      struct Cmd_Node *cmd)
{
    skip_newline(lexer);
    enum TokenType NextToken = get_nextToken(lexer);

    if (NextToken == TOKEN_FOR)
    {
        lexer_advance(lexer);
        list->done_condition = (struct Ast *)parse_ForCommand(lexer);
    }

    else if (NextToken != TOKEN_WORD && NextToken != TOKEN_TRUE
             && NextToken != TOKEN_FALSE && NextToken != TOKEN_NOT)
    {
        lexer_advance(lexer);
        fprintf(stderr, "42sh: syntax error near unexpected token `%s'\n",
                lexer->current_token.value);
        return NULL;
    }

    else
    {
        if (!no)
        {
            list->do_condition =
                (struct Ast *)parse_CommandList(lexer, cmd->variable);
        }

        else
        {
            enum TokenType next = get_nextToken(lexer);

            while (next != TOKEN_SEMICOLON)
            {
                lexer_advance(lexer);
                next = get_nextToken(lexer);
            }

            lexer_advance(lexer);
        }
    }

    lexer_advance(lexer);

    if (lexer->current_token.type != TOKEN_DONE)
    {
        fprintf(stderr, "42sh: syntax error near unexpected token `done'\n");
        return NULL;
    }

    lexer_advance(lexer);

    return list;
}

int verify_and_or(struct Lexer *lexer, struct Cmd_Node *cmd)
{
    enum TokenType next = get_nextToken(lexer);
    int res = 0;

    if (next == TOKEN_AND)
    {
        if (lexer->current_token.type == TOKEN_FALSE && cmd->and_or != 0)
            cmd->exclamation = 1;

        lexer_advance(lexer);
        next = get_nextToken(lexer);

        if (next == TOKEN_FALSE)
            cmd->exclamation = 1;

        else if (next == TOKEN_TRUE && cmd->and_or != 1)
            cmd->exclamation = 0;

        else if (next == TOKEN_AND || next == TOKEN_EOF
                 || next == TOKEN_SEMICOLON)
            return 2;
    }

    else if (next == TOKEN_OR)
        verify_or(lexer, cmd, &res);

    else
        lexer_advance(lexer);

    return res;
}

void verify_or(struct Lexer *lexer, struct Cmd_Node *cmd, int *res)
{
    enum TokenType next;

    if ((lexer->current_token.type == TOKEN_FALSE && cmd->and_or == 0))
    {
        cmd->exclamation = 0;
        cmd->and_or = 3;
    }

    else if ((lexer->current_token.type == TOKEN_TRUE && cmd->and_or != 1))
    {
        cmd->exclamation = 0;
        cmd->and_or = 2;
    }

    lexer_advance(lexer);
    next = get_nextToken(lexer);

    if (next == TOKEN_TRUE)
        cmd->exclamation = 0;

    else if (next == TOKEN_FALSE && cmd->and_or == 1)
        cmd->exclamation = 1;

    else if (next == TOKEN_WORD)
    {
        if (cmd->exclamation == 1)
            cmd->and_or = 1;

        if (lexer->current_token.type == TOKEN_FALSE)
            cmd->exclamation = 1;
        else
            cmd->exclamation = 0;
    }

    else if (next == TOKEN_NOT)
    {
        if (cmd->exclamation == 0)
        {
            cmd->and_or = 0;
            cmd->exclamation = 0;
        }

        else
        {
            cmd->and_or = 1;
            cmd->exclamation = 1;
        }
    }

    else if (next == TOKEN_OR || next == TOKEN_EOF || next == TOKEN_SEMICOLON)
        *res = 2;
}

struct While_Node *parse_WhileCommand(struct Lexer *lexer)
{
    struct While_Node *list = my_calloc(1, sizeof(struct While_Node));
    list->node.type = WHILE;

    skip_newline(lexer);
    list->do_condition = (struct Ast *)parse_CommandList(lexer, NULL);

    if (!list->do_condition)
        return NULL;

    lexer_advance(lexer);

    if (lexer->current_token.type != TOKEN_DO)
    {
        fprintf(stderr, "42sh: syntax error near unexpected token `do'\n");
        return NULL;
    }

    else
    {
        skip_newline(lexer);
        enum TokenType NextToken = get_nextToken(lexer);

        if (NextToken == TOKEN_WHILE || NextToken == TOKEN_UNTIL)
        {
            lexer_advance(lexer);
            list->done_condition = (struct Ast *)parse_WhileCommand(lexer);
        }

        else if (NextToken != TOKEN_WORD && NextToken != TOKEN_TRUE
                 && NextToken != TOKEN_FALSE)
        {
            lexer_advance(lexer);
            fprintf(stderr, "42sh: syntax error near unexpected token `%s'\n",
                    lexer->current_token.value);
            return NULL;
        }

        else
        {
            list->done_condition = (struct Ast *)parse_CommandList(lexer, NULL);
        }

        lexer_advance(lexer);

        if (lexer->current_token.type != TOKEN_DONE)
        {
            fprintf(stderr,
                    "42sh: syntax error near unexpected token `done'\n");
            return NULL;
        }

        lexer_advance(lexer);
    }

    return list;
}
