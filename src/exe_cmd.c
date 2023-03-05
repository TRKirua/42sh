#include "exe_cmd.h"

void execute_pipe(struct Cmd_Node *cmd_node)
{
    char ***list = cmd_node->words;

    int pipe_fd[2];
    int status;
    pid_t pid;

    int i = 0;

    while (i < cmd_node->pipe)
    {
        pipe(pipe_fd);
        pid = fork();

        if (pid == 0)
        {
            dup2(pipe_fd[1], STDOUT_FILENO);
            close(pipe_fd[0]);
            close(pipe_fd[1]);

            if (execvp(list[i][0], list[i]) == -1)
            {
                fprintf(stderr, "42sh: %s: command not found\n", list[i][0]);
                exit(127);
            }
        }

        else
        {
            close(pipe_fd[1]);
            dup2(pipe_fd[0], STDIN_FILENO);
            close(pipe_fd[0]);
            waitpid(pid, &status, 0);

            if (WEXITSTATUS(status) == 127)
                exit(127);
            i++;
        }
    }

    cmd_node->pipe--;

    if (execvp(list[i][0], list[i]) == -1)
    {
        fprintf(stderr, "42sh: %s: command not found\n", list[i][0]);
        exit(127);
    }
}

int cmd_result(struct Ast *ast)
{
    struct Cmd_Node *cmd_node = (struct Cmd_Node *)ast;
    char ***list = cmd_node->words;

    int result = 0;

    for (int i = 0; i < cmd_node->NbrCmd; i++)
    {
        if (cmd_node->pipe != 0)
            execute_pipe(cmd_node);

        if (!strcmp(list[i][0], "echo"))
        {
            result = echo_builtin(list[i]);
        }

        else if (!strcmp(list[i][0], "cd"))
        {
            if ((result = cd_builtin(list[i])))
                return result;
        }

        else if (!strcmp(list[i][0], "."))
        {
            if ((result = dot_builtin(list[i])))
                return result;
        }

        else if (!strcmp(list[i][0], "export"))
        {
            if ((result = export_builtin(list[i])))
                return result;
        }

        else if (!strcmp(list[i][0], "true") || !strcmp(list[i][0], "false"))
        {
            if (i + 1 == cmd_node->NbrCmd)
                return cmd_node->exclamation;
        }

        else
        {
            int childPid = fork();
            int status;

            if (childPid == -1)
                return -1;

            if (childPid == 0)
            {
                int result = execvp(list[i][0], list[i]);

                if (result == -1)
                {
                    fprintf(stderr, "42sh: %s: command not found\n",
                            list[i][0]);
                    exit(127);
                }

                exit(result);
            }

            else
            {
                if (waitpid(childPid, &status, 0) < 0)
                    return 1;

                return WEXITSTATUS(status);
            }
        }
    }

    result = cmd_node->exclamation;
    return result;
}

int if_result(struct Ast *ast)
{
    ast->then_true = 0;
    int then_true = 0;

    struct If_Node *if_cmd = (struct If_Node *)ast;
    struct Ast *cmd_node_if = if_cmd->if_condition;

    int if_res = cmd_result(cmd_node_if);

    if (if_res == 0)
    {
        struct Ast *cmd_node_then = if_cmd->then_condition;
        int then_res = 0;

        if (cmd_node_then->type == COMMAND_LIST)
        {
            then_res = cmd_result(cmd_node_then);
        }

        else if (cmd_node_then->type == IF)
        {
            then_res = execute_Ast(cmd_node_then);
        }

        ast->then_true = 1;
        return then_res;
    }

    if (if_cmd->elif_condition != NULL)
    {
        struct Ast *cmd_node_elif = if_cmd->elif_condition;

        int elif_res = execute_Ast(cmd_node_elif);
        then_true = cmd_node_elif->then_true;

        if (elif_res == 2)
            return 2;
    }

    if (if_res == 1 && !then_true && if_cmd->else_condition != NULL)
    {
        struct Ast *cmd_node_else = if_cmd->else_condition;
        int else_res = 0;

        if (cmd_node_else->type == COMMAND_LIST)
        {
            else_res = cmd_result(cmd_node_else);
        }

        else if (cmd_node_else->type == IF)
        {
            else_res = execute_Ast(cmd_node_else);
        }

        if (else_res == 2)
            return 2;

        if_res = else_res;
    }

    return if_res;
}

int while_result(struct Ast *ast)
{
    struct While_Node *while_cmd = (struct While_Node *)ast;
    struct Ast *cmd_node_while = while_cmd->do_condition;

    int while_res = cmd_result(cmd_node_while);

    if (while_res == 0)
    {
        while (while_res == 0)
        {
            struct Ast *cmd_node_done = while_cmd->done_condition;
            int done_res = 0;

            if (cmd_node_done->type == COMMAND_LIST)
            {
                done_res = cmd_result(cmd_node_done);
            }

            else if (cmd_node_done->type == WHILE)
            {
                done_res = execute_Ast(cmd_node_done);
            }

            if (done_res == 2)
                return 2;

            while_res = cmd_result(cmd_node_while);
        }
    }

    else if (while_res == 1)
        while_res = 0;

    return while_res;
}

int for_result(struct Ast *ast)
{
    struct For_Node *for_cmd = (struct For_Node *)ast;
    struct Ast *cmd_node_for = for_cmd->do_condition;

    int res = 0;
    res = cmd_result(cmd_node_for);

    return res;
}
