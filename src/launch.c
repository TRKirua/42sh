#include "launch.h"

int launch_shell(void) // When user types : ./42sh
{
    int exe = 0;

    while (1)
    {
        char *cmdLine;
        cmdLine = readCommandLine();

        if (!cmdLine)
            break;

        if (!strcmp(cmdLine, "exit"))
        {
            printf("exit\n");
            free(cmdLine);
            break;
        }

        if (strcmp(cmdLine, "\0"))
        {
            exe = executeCommand(cmdLine);
            free(cmdLine);
        }

        else
            free(cmdLine);
    }

    return exe;
}

int input_quotes(char *input) // When user types : ./42sh -c "INPUT"
{
    return executeCommand(input);
}

int input_file(char *filename) // When user types : ./42sh FILE
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        perror("Could not open file");
        return 2;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char *str = (char *)malloc((size + 1) * sizeof(char));
    fread(str, sizeof(char), size, file);
    str[size] = '\0';

    int exe = executeCommand(str);
    free(str);

    fclose(file);

    return exe;
}

int input_stdin(void) // When user types : ./42sh < FILE
{
    int exe = 0;

    char *str = NULL;
    size_t size = 0;
    char c;

    while ((c = getchar()) != EOF)
    {
        str = (char *)realloc(str, size + 1);
        str[size++] = c;
    }

    str = (char *)realloc(str, size + 1);
    str[size] = '\0';

    exe = executeCommand(str);
    free(str);

    return exe;
}
