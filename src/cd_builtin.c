#include "cd_builtin.h"

int cd_builtin(char **cmd)
{
    char *dir = NULL;
    struct stat sb;

    if (!cmd[1])
        dir = getenv("HOME");

    else if (!strcmp(cmd[1], "-"))
        dir = getenv("OLDPWD");

    else
        dir = cmd[1];

    if (lstat(dir, &sb))
    {
        fprintf(stderr, "42sh: cd: %s: No such file or directory\n", dir);
        return 1;
    }

    if (!S_ISDIR(sb.st_mode))
    {
        fprintf(stderr, "42sh: cd: %s: Not a directory\n", dir);
        return 1;
    }

    if (chdir(dir))
    {
        fprintf(stderr, "42sh: cd: %s: %s\n", dir, strerror(errno));
        return -1;
    }

    else
    {
        setenv("OLDPWD", getenv("PWD"), 1);
        setenv("PWD", dir, 1);
        return 0;
    }
}
