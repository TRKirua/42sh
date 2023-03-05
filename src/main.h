#ifndef MAIN_H
#define MAIN_H

#define _POSIX_C_SOURCE 200809L
#define MAX_TOKENS_PER_LIST 64

#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "astruct.h"
#include "cd_builtin.h"
#include "dot_builtin.h"
#include "echo_builtin.h"
#include "exe.h"
#include "exe_cmd.h"
#include "export_builtin.h"
#include "launch.h"
#include "lexer.h"
#include "mmalloc.h"
#include "parser.h"
#include "parser_two.h"

#endif // ! MAIN_H
