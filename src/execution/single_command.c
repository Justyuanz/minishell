#include "minishell.h"

void    execute_single_command(t_shell *shell)
{
    char    *command_path;
    t_cmd   *cmd;

    cmd = get_cmd(shell->data, 0);

    command_path = get_command_path(cmd->argv[0], shell);
    if (command_path)
    {
        if (execve(command_path, cmd->argv, shell->envp) == -1)
        {
            free(command_path);
            command_path = NULL;
        }
    }
    //cleanup_child(shell);
    exit(shell->exitcode);
}

void    handle_single_command(t_shell *shell)
{
    //ignore signal
    shell->pids[0] = fork();
    if (shell->pids[0] < 0)
        error_smt();
    if (shell->pids[0] == 0)
        execute_single_command(shell);
    /* else
   {
        wait_for_all(shell);
    }
        */
    //singal here
}


void    single_command_case(t_shell *shell)
{
    int flag;
    t_cmd   *cmd;
    if (!shell) {
        printf("ERROR: shell is NULL\n");
        return;
    }
    
    if (!shell->data) {
        printf("ERROR: shell->data is NULL\n");
        return;
    }

    cmd = get_cmd(shell->data, 0);
    // can here be heredoc?
    if (cmd)
    {
        flag = check_if_builtin(cmd->argv[0]);
        if (flag != 0)
            handle_builtin(flag, cmd, shell);
        else // should i check here for valid command or not??
            handle_single_command(shell);
    }
}
