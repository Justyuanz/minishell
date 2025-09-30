#include "minishell.h"

void    execute_single_command(t_command *command, t_shell *shell)
{
    /*
        get the command_path and run execve
    */
}

void    handle_single_command(t_command *command, t_shell *shell)
{
    shell->pids[0] = fork();
    if (shell->pids[0] < 0)
        error_smt();
    if (shell->pids[0] == 0)
        execute_single_command(command, shell);
    else
    {
        /*
            what to do for parent process??
        */
    }
}

void    single_command_case(t_command *command, t_shell *shell)
{
    int flag;
    // can here be heredoc?
    if (command->command_array)
    {
        flag = check_if_builtin(command->command_array[0]);
        if (flag != 0)
            handle_builtin(flag, command, shell);
        else // should i check here for valid command or not??
            handle_single_command(command, shell);
    }
}