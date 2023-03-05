#include "lexer.h"

struct Token token_pairs[MAX_TOKENS] = {
    { TOKEN_IF, "if" },       { TOKEN_THEN, "then" },
    { TOKEN_ELIF, "elif" },   { TOKEN_ELSE, "else" },
    { TOKEN_FI, "fi" },       { TOKEN_SEMICOLON, ";" },
    { TOKEN_PIPE, "|" },      { TOKEN_NEWLINE, "\n" },
    { TOKEN_WHILE, "while" }, { TOKEN_DO, "do" },
    { TOKEN_AMPER, "&" },     { TOKEN_OR, "||" },
    { TOKEN_NOT, "!" },       { TOKEN_AND, "&&" },
    { TOKEN_DONE, "done" },   { TOKEN_FOR, "for" },
    { TOKEN_UNTIL, "until" }, { TOKEN_TRUE, "true" },
    { TOKEN_FALSE, "false" }, { TOKEN_REDIR, ">" },
    { TOKEN_REDIR2, ">>" },   { TOKEN_UNKNOWN, NULL }
};

enum TokenType get_nextToken(struct Lexer *lexer)
{
    int i = lexer->index;
    char *input = lexer->input;

    while (isspace(input[i]) && input[i] != '\n')
        i++;

    while (input[i] == '#')
    {
        while (input[i] != '\n' && input[i] != '\0')
            i++;

        if (input[i] == '\0')
            return TOKEN_EOF;

        i++;
    }

    if (input[i] == '\0')
        return TOKEN_EOF;

    if (input[i] == '\n')
        return TOKEN_NEWLINE;

    enum TokenType token = TOKEN_WORD;

    for (int k = 0; k < MAX_TOKENS; k++)
    {
        int length = strcspn(&input[i], " ;\n");
        ;
        if (length == 0 || length == 1)
        {
            length = 1;
        }

        if (token_pairs[k].value != NULL && input[i] != '\0'
            && strncmp(&input[i], token_pairs[k].value, length) == 0)
        {
            token = token_pairs[k].type;
        }
    }

    if (token != TOKEN_WORD)
        return token;

    char *nextWord = malloc(sizeof(char));
    int j = 0;

    while (input[i] != ';' && input[i] != ' ' && input[i] != '\0'
           && input[i] != '\n')
    {
        nextWord[j++] = lexer->input[i++];
        nextWord = realloc(nextWord, sizeof(char) * (j + 1));
    }

    nextWord[j] = '\0';
    free(nextWord);
    return token;
}

char *my_strndup(const char *s, size_t n)
{
    size_t len = 0;
    const char *temp = s;

    while (len < n && *temp)
    {
        ++len;
        ++temp;
    }

    char *result = (char *)my_malloc(len + 1);
    if (!result)
        return NULL;
    memcpy(result, s, len);
    result[len] = '\0';
    return result;
}

char *my_strdup(char *s)
{
    int len = strlen(s) + 1;
    char *dup = my_malloc(len);

    if (dup)
        strcpy(dup, s);
    return dup;
}

void lexer_init(struct Lexer *lexer, char *input)
{
    lexer->input = input;
    lexer->index = 0;
    lexer->current_token.value = NULL;
}

int lexer_quote(struct Lexer *lexer, int wword, char *input)
{
    int quote_start = -1;

    if (input[lexer->index] == '\"')
    {
        quote_start = lexer->index;
        lexer->index++;

        while (input[lexer->index] != '\"' && input[lexer->index] != '\0')
        {
            lexer->index++;
        }

        if (input[lexer->index] == '\"')
        {
            int word_length = lexer->index - quote_start - 1;
            lexer->current_token.value =
                my_strndup(&input[quote_start + 1], word_length);
            lexer->current_token.type = TOKEN_WORD;
            lexer->index++;
        }

        wword = 0;
        return wword;
    }

    else if (input[lexer->index] == '\'')
    {
        quote_start = lexer->index;
        lexer->index++;

        while (input[lexer->index] != '\'' && input[lexer->index] != '\0')
        {
            lexer->index++;
        }

        if (input[lexer->index] == '\'')
        {
            int word_length = lexer->index - quote_start - 1;
            lexer->current_token.value =
                my_strndup(&input[quote_start + 1], word_length);
            lexer->current_token.type = TOKEN_WORD;
            lexer->index++;
        }

        wword = 0;
        return wword;
    }

    return wword;
}

int find_basic(struct Lexer *lexer, int wword, char *input)
{
    for (int i = 0; token_pairs[i].value; i++)
    {
        size_t length = strcspn(&input[lexer->index], " ;>|&\n");

        if (input[lexer->index] == '&' && input[lexer->index + 1] == '&')
        {
            length = strcspn(&input[lexer->index], " ;>|&\n") + 2;
        }

        else if (input[lexer->index] == '|' && input[lexer->index + 1] == '|')
        {
            length = strcspn(&input[lexer->index], " ;>|&\n") + 2;
        }

        else if (input[lexer->index] == '>' && input[lexer->index + 1] == '>')
        {
            length = strcspn(&input[lexer->index], " ;>|&\n") + 2;
        }

        if (length == 0 || length == 1)
        {
            length = 1;
        }

        if (token_pairs[i].value != NULL && input[lexer->index] != '\0'
            && strncmp(token_pairs[i].value, &input[lexer->index], length) == 0)
        {
            if (strncmp(token_pairs[i].value, &input[lexer->index], length) == 0
                && length != strlen(token_pairs[i].value))
            {
                continue;
            }

            lexer->current_token.value =
                my_strndup(&input[lexer->index], strlen(token_pairs[i].value));
            lexer->current_token.type = token_pairs[i].type;
            lexer->index += strlen(token_pairs[i].value);
            wword = 0;
            return wword;
        }
    }

    return wword;
}

void lexer_advance(struct Lexer *lexer)
{
    char *input = lexer->input;
    unsigned int wword = 1;
    lexer->current_token.value = NULL;

    while (isspace(input[lexer->index]) && input[lexer->index] != '\n')
    {
        lexer->index++;
    }

    while (input[lexer->index] == '#')
    {
        while (input[lexer->index] != '\n' && input[lexer->index] != '\0')
            lexer->index++;

        if (input[lexer->index] == '\0')
        {
            lexer->current_token.type = TOKEN_EOF;
            return;
        }

        lexer->index++;
    }

    if (input[lexer->index] == '\0')
    {
        lexer->current_token.type = TOKEN_EOF;
        return;
    }

    wword = find_basic(lexer, wword, input);
    wword = lexer_quote(lexer, wword, input);

    if (wword == 1)
    {
        lexer->current_token.type = TOKEN_WORD;
        int start = lexer->index;

        while (input[lexer->index] != ' ' && input[lexer->index] != '\0')
        {
            if (input[lexer->index] == ';' || input[lexer->index] == '>'
                || input[lexer->index] == '&' || input[lexer->index] == '|'
                || input[lexer->index] == '\n')
            {
                break;
            }

            lexer->index++;
        }

        int length = lexer->index - start;
        lexer->current_token.value = my_strdup(&input[start]);
        lexer->current_token.value[length] = '\0';
        return;
    }
}
