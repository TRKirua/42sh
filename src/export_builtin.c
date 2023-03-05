#include "export_builtin.h"

int export_builtin(char **cmd)
{
    if (cmd[1] == NULL || cmd[2] != NULL)
    {
        fprintf(stderr, "export: %s: not a valid identifier\n", cmd[1]);
        return 2;
    }

    char *var = strtok(cmd[1], "=");
    char *val = strtok(NULL, "=");

    if (var == NULL)
    {
        fprintf(stderr, "export: %s: not a valid identifier\n", cmd[1]);
        return 2;
    }

    if (val == NULL)
    {
        val = getenv(var);

        if (val == NULL)
        {
            fprintf(stderr, "export: %s: not found\n", var);
            return 1;
        }
    }

    if (setenv(var, val, 1) != 0)
    {
        fprintf(stderr, "export: %s\n", strerror(errno));
        return 1;
    }

    return 0;
}
