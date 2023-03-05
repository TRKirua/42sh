#include "echo_builtin.h"

// \n a la fin
int echo_print_newline(char **cmd, int i, int nb_opt)
{
    if (i - nb_opt == 1) // echo NULL |echo -n NULL
    {
        printf("\n");
        return 0;
    }

    int index = nb_opt + 1;

    while (cmd[index + 1])
    {
        printf("%s ", cmd[index]);
        index++;
    }

    printf("%s\n", cmd[index]);

    return 0;
}

// pas de \n a la fin
int echo_print_no_newline(char **cmd, int i, int nb_opt)
{
    if (i - nb_opt == 1)
        return 0;

    int index = nb_opt + 1;

    while (cmd[index + 1])
    {
        printf("%s ", cmd[index]);
        index++;
    }

    printf("%s", cmd[index]);

    return 0;
}

// echo -e, pas de \n a la fin, il faut l'ajouter sois meme
int echo_enable(char **cmd, int i, int nb_opt)
{
    if (i - nb_opt == 1)
        return 0;

    int index = nb_opt + 1;

    while (cmd[index] != NULL)
    {
        int k = 0;

        while (cmd[index][k] != '\0')
        {
            if (cmd[index][k] == '\\')
            {
                if (cmd[index][k + 1] != '\0')
                {
                    if (cmd[index][k + 1] == 'n')
                        printf("\n");

                    else if (cmd[index][k + 1] == 't')
                        printf("\t");

                    else if (cmd[index][k + 1] == '\\')
                        printf("\\");

                    else
                        printf("\\%c", cmd[index][k + 1]);

                    k++;
                }
            }

            else
                printf("%c", cmd[index][k]);

            k++;
        }

        index++;
    }

    return 0;
}

// Retourne l'option de echo en int
int echo_option(char **cmd, int *nb_flags)
{
    if (*nb_flags == 0)
        return 1;

    int flag_e = 0;
    int flag_E = 0;
    int flag_n = 0;

    for (int j = 1; j < *nb_flags + 1; j++)
    {
        int k = 1;

        while (cmd[j][k] != '\0')
        {
            if (cmd[j][k] != 'e' && cmd[j][k] != 'E' && cmd[j][k] != 'n')
            {
                *nb_flags = j - 1;

                if (flag_E + flag_e + flag_n == 0)
                    return 1;
                return flag_E + flag_e + flag_n;
            }

            k++;
        }

        k = 1;

        while (cmd[j][k] != '\0')
        {
            if (cmd[j][k] == 'e')
            {
                flag_e = 3;
                flag_E = 0;
            }

            else if (cmd[j][k] == 'E')
            {
                if (flag_e != 3)
                {
                    flag_e = 0;
                    flag_E = 5;
                }
            }

            else if (cmd[j][k] == 'n')
            {
                flag_n = 10;
            }

            k++;
        }
    }

    return flag_E + flag_e + flag_n; // 3 | 5 | 10 | 13 | 15
}

int echo_builtin(char **cmd) // Ex = [ ["echo"], ["-n"], ["oui"], NULL ]
{
    int i = 1;
    int nb_flags = 0;
    short stop_flag = 0;

    while (cmd[i] != NULL)
    {
        if (cmd[i][0] == '-' && stop_flag == 0)
            nb_flags++;
        else
            stop_flag = 1;
        i++;
    }

    int option = echo_option(cmd, &nb_flags);
    int r = 0;

    if (option == 1 || option == 5) // no_option OR -E
        r = echo_print_newline(cmd, i, nb_flags);

    else if (option == 3) // -e
    {
        r = echo_enable(cmd, i, nb_flags);
        printf("\n");
    }

    else if (option == 10 || option == 15) // -n OR -nE
        r = echo_print_no_newline(cmd, i, nb_flags);

    else if (option == 13) // -ne
        r = echo_enable(cmd, i, nb_flags);

    fflush(stdout);

    return r;
}
