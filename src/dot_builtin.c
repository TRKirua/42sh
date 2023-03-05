#include "dot_builtin.h"

int dot_builtin(char **cmd)
{
    if (!cmd[1])
    {
        fprintf(stderr, "42sh: .: filename argument required\n");
        return 2;
    }

    if (access(cmd[1], F_OK) == -1) // File does not exist
    {
        fprintf(stderr, "42sh: %s: file not found\n", cmd[1]);
        return 1;
    }

    int child = fork();
    int res = 0;

    if (child == 0)
    {
        res = input_file(cmd[1]);
        exit(res);
    }

    else
    {
        waitpid(child, NULL, 0);
        return 0;
    }
}
