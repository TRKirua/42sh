#ifndef ECHO_BUILTIN_H
#define ECHO_BUILTIN_H

#include "main.h"

int echo_print_newline(char **cmd, int i, int nb_op);
int echo_print_no_newline(char **cmd, int i, int nb_op);
int echo_enable(char **cmd, int i, int nb_op);
int echo_option(char **cmd, int *nb_flags);
int echo_builtin(char **cmd);

#endif // ! ECHO_BUILTIN_H
