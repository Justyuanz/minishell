#include "minishell.h"

void    handle_execution(t_command *command, t_shell *shell)
{
    /*
        get command path and do execve
    */
}

void    handle_pipes(t_shell *shell)
{
    /*
        dup2 and close all pipes
    */
}

void    do_command_fork(t_command *command, t_shell *shell)
{
    int flag_builtin;

    shell->pids[0] = fork(); // thinking about switching 0 to some index, but what and where to get it???
    if (shell->pids[0] < 0)
        return ;
    if (shell->pids[0] == 0)
    {
        handle_pipes(shell); //not working function yet
        if (command->type != WORD)
        {
            //some error handling
        }
        flag_builtin = check_if_builtin(command->command_array[0]);
        if (flag_builtin != 0)
            handle_builtin(flag_builtin, command, shell);
        else
            handle_execution(command, shell); // not working yet
    }
    // do something for fork == 1
}

void    handle_command(t_command *command, t_shell *shell)
{
    if (command->command_array)
    {
        if (check_valid_command(command, shell))
        {
            if (command->type != WORD) // WORD is for commands???
            {
                // need to fix this if. Maybe its a heredoc or something idk
               ft_putstr_fd("error\n", 2);
               return ;
            }
            do_command_fork(command, shell);
        }
    }
}