#ifndef EXE_CMD_H
#define EXE_CMD_H

#include "main.h"

int cmd_result(struct Ast *ast);
int if_result(struct Ast *ast);
int while_result(struct Ast *ast);
int for_result(struct Ast *ast);
void execute_pipe(struct Cmd_Node *cmd_node);

#endif // ! EXE_CMD_H
