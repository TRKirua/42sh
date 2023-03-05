#include "parser.h"

void skip_newline(struct Lexer *lexer)
{
    while (get_nextToken(lexer) == TOKEN_NEWLINE)
        lexer_advance(lexer);
}

struct Ast *parse(struct Lexer *lexer)
{
    skip_newline(lexer);
    enum TokenType type = get_nextToken(lexer);

    struct Ast *head = NULL;

    if (type == TOKEN_IF)
    {
        lexer_advance(lexer); // curently on the if
        head = (struct Ast *)parse_IfCommand(lexer, 1);

        if (get_nextToken(lexer) == TOKEN_FI)
        {
            if (head)
                fprintf(stderr,
                        "42sh: syntax error near unexpected token `fi'\n");
            return NULL;
        }
    }

    else if (type == TOKEN_WHILE || type == TOKEN_UNTIL)
    {
        lexer_advance(lexer);
        head = (struct Ast *)parse_WhileCommand(lexer);
    }

    else if (type == TOKEN_FOR)
    {
        lexer_advance(lexer);
        head = (struct Ast *)parse_ForCommand(lexer);
    }

    else if (type == TOKEN_AND || type == TOKEN_OR)
    {
        lexer_advance(lexer);
        fprintf(stderr, "42sh: syntax error near unexpected token `%s'\n",
                lexer->current_token.value);
        return NULL;
    }

    else if (type != TOKEN_EOF)
    {
        head = (struct Ast *)parse_CommandList(lexer, NULL);
        if (!head)
            fprintf(stderr, "42sh: syntax error near unexpected token `|'\n");
    }

    return head;
}

struct Table *init_table(void)
{
    struct Table *table = my_malloc(sizeof(struct Table));
    table->i = 0;
    table->j = 0;
    table->semicolon = 0;
    table->stop = 0;
    table->true_false_ = 1;
    table->incr = 1;
    table->last = 0;
    return table;
}

struct Cmd_Node *parse_CommandList(struct Lexer *lexer, char **var)
{
    lexer_advance(lexer);
    enum TokenType next = get_nextToken(lexer);

    if (lexer->current_token.type == TOKEN_SEMICOLON)
    {
        if (next == TOKEN_SEMICOLON)
            fprintf(stderr, "42sh: syntax error near unexpected token `;;'\n");
        else
            fprintf(stderr, "42sh: syntax error near unexpected token `;'\n");
        return NULL;
    }

    struct Cmd_Node *Cmd = my_malloc(sizeof(struct Cmd_Node));
    Cmd->node.type = COMMAND_LIST;

    Cmd->variable = var;

    Cmd->words = (char ***)my_malloc(sizeof(char **));
    Cmd->NbrCmd = 1;

    Cmd->exclamation = 0;
    Cmd->and_or = 0;
    Cmd->pipe = 0;

    struct Table *table = init_table();

    Cmd->words[table->i] = my_malloc(sizeof(char *));

    while (lexer->current_token.type != TOKEN_EOF)
    {
        if (lexer->current_token.type == TOKEN_SEMICOLON
            || lexer->current_token.type == TOKEN_AND
            || lexer->current_token.type == TOKEN_OR
            || lexer->current_token.type == TOKEN_NEWLINE
            || lexer->current_token.type == TOKEN_PIPE)
        {
            int res = parse_CommandList_sub(lexer, table, Cmd);
            if (res == 2)
                return NULL;
            if (res == 1)
                break;
        }

        else
        {
            parse_CommandList_sub_sub(lexer, table, Cmd);
        }

        int res = verify_and_or(lexer, Cmd);
        if (res == 2)
            return NULL;
    }

    if (!table->semicolon)
        Cmd->words[table->i][table->j] = NULL;

    return Cmd;
}

int parse_CommandList_sub(struct Lexer *lexer, struct Table *table,
                          struct Cmd_Node *Cmd)
{
    if (lexer->current_token.type == TOKEN_SEMICOLON && table->last != 0)
    {
        lexer->index -= table->last;
        table->last = 0;
    }

    enum TokenType next;

    if (lexer->current_token.type == TOKEN_PIPE)
    {
        Cmd->pipe++;
        next = get_nextToken(lexer);
        if (next == TOKEN_EOF || next == TOKEN_OR)
            return 2;
    }

    if (!table->stop)
        Cmd->words[table->i++][table->j] = NULL;

    table->semicolon = 1;
    table->j = 0;
    table->stop = 0;
    table->true_false_ = 1;

    skip_newline(lexer);
    next = get_nextToken(lexer);

    if (Cmd->exclamation == 1 && lexer->current_token.type == TOKEN_AND)
        table->stop = 1;

    else if (Cmd->and_or != 1 && lexer->current_token.type == TOKEN_OR)
        table->stop = 1;

    else if ((next == TOKEN_WORD || next == TOKEN_FALSE || next == TOKEN_TRUE))
    {
        Cmd->words = my_realloc(Cmd->words, (table->i + 1) * sizeof(char **));
        Cmd->words[table->i] = (char **)my_malloc(sizeof(char *));

        Cmd->NbrCmd++;

        if (lexer->current_token.type != TOKEN_AND
            && lexer->current_token.type != TOKEN_OR)
            Cmd->exclamation = 0;

        table->semicolon = 0;
    }

    else
    {
        if (next == TOKEN_SEMICOLON
            && lexer->current_token.type == TOKEN_SEMICOLON)
        {
            Cmd = NULL;
            fprintf(stderr, "42sh: syntax error near unexpected token `;;'\n");
        }

        return 1;
    }

    return 0;
}

void parse_CommandList_sub_sub(struct Lexer *lexer, struct Table *table,
                               struct Cmd_Node *Cmd)
{
    if (!table->stop)
    {
        Cmd->and_or = 0;

        if (table->true_false_)
        {
            exclamation_change(lexer, Cmd);
            table->true_false_ = 0;
        }

        if (Cmd->variable != NULL)
            compare_for(&table->last, &lexer->current_token.value, Cmd,
                        &table->incr);

        int len = strlen(lexer->current_token.value);

        Cmd->words[table->i][table->j] = (char *)my_malloc(len + 1);
        strncpy(Cmd->words[table->i][table->j], lexer->current_token.value,
                len);

        Cmd->words[table->i][table->j++][len] = '\0';

        Cmd->words[table->i] = (char **)my_realloc(
            Cmd->words[table->i], (table->j + 1) * sizeof(char *));
    }
}

struct If_Node *parse_IfCommand(struct Lexer *lexer, int need_fi)
{
    struct If_Node *list = my_calloc(1, sizeof(struct If_Node));
    list->node.type = IF;

    skip_newline(lexer);
    list->if_condition = (struct Ast *)parse_CommandList(lexer, NULL);

    if (!list->if_condition)
        return NULL;

    lexer_advance(lexer);

    if (lexer->current_token.type != TOKEN_THEN)
    {
        fprintf(stderr, "42sh: syntax error near unexpected token `then'\n");
        return NULL;
    }

    else
    {
        skip_newline(lexer);
        enum TokenType NextToken = get_nextToken(lexer);

        if (NextToken == TOKEN_IF)
        {
            lexer_advance(lexer);
            list->then_condition = (struct Ast *)parse_IfCommand(lexer, 1);
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
            list->then_condition = (struct Ast *)parse_CommandList(lexer, NULL);
        }

        lexer_advance(lexer);

        if (lexer->current_token.type == TOKEN_ELIF)
        {
            list = parse_IfCommand_sub_sub(lexer, list);
        }

        if (!need_fi)
            return list;

        if (lexer->current_token.type == TOKEN_ELSE)
        {
            list = parse_IfCommand_sub(lexer, list);
        }

        if (lexer->current_token.type != TOKEN_FI)
        {
            fprintf(stderr, "42sh: syntax error near unexpected token `fi'\n");
            return NULL;
        }
    }

    lexer_advance(lexer);
    return list;
}

struct If_Node *parse_IfCommand_sub(struct Lexer *lexer, struct If_Node *list)
{
    skip_newline(lexer);
    enum TokenType NextToken = get_nextToken(lexer);

    if (NextToken == TOKEN_IF)
    {
        lexer_advance(lexer);
        list->else_condition = (struct Ast *)parse_IfCommand(lexer, 1);
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
        list->else_condition = (struct Ast *)parse_CommandList(lexer, NULL);
    }

    lexer_advance(lexer);
    return list;
}

struct If_Node *parse_IfCommand_sub_sub(struct Lexer *lexer,
                                        struct If_Node *list)
{
    skip_newline(lexer);
    enum TokenType NextToken = get_nextToken(lexer);

    if (NextToken != TOKEN_WORD && NextToken != TOKEN_TRUE
        && NextToken != TOKEN_FALSE)
    {
        lexer_advance(lexer);
        fprintf(stderr, "42sh: syntax error near unexpected token `%s'\n",
                lexer->current_token.value);
        return NULL;
    }

    list->elif_condition = (struct Ast *)parse_IfCommand(lexer, 0);

    return list;
}

void exclamation_change(struct Lexer *lexer, struct Cmd_Node *Cmd)
{
    if (lexer->current_token.type == TOKEN_FALSE && Cmd->and_or != 2)
    {
        Cmd->and_or = 1;
        Cmd->exclamation = 1;
    }

    if (lexer->current_token.type == TOKEN_NOT)
    {
        lexer_advance(lexer);

        if (lexer->current_token.type == TOKEN_FALSE)
        {
            Cmd->and_or = 0;
            Cmd->exclamation = 0;
        }
        else if (lexer->current_token.type == TOKEN_TRUE && Cmd->and_or != 3)
        {
            Cmd->and_or = 1;
            Cmd->exclamation = 1;
        }
        else if (lexer->current_token.type == TOKEN_WORD)
        {
            Cmd->exclamation = ((Cmd->exclamation == 0) ? 1 : 0);
        }
    }
}
