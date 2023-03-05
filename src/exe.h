#ifndef EXE_H
#define EXE_H

#include "exe_cmd.h"
#include "main.h"

char *readCommandLine(void);
int execute_Ast(struct Ast *ast);
int executeCommand(char *line);

#endif // ! EXE_H
