#include "minishell.h"
/*
void    shell_init(char **envp, t_shell *shell)
{
    shell->exitcode = 0;
    shell->index = 0;
    shell->command_index = 0;
    shell->input = STDIN_FILENO;
    shell->output = STDOUT_FILENO;
}

void    handle_execution(t_command *command, t_shell *shell)
{
    
        get command path and do execve
    //
}

void    do_command_fork(t_command *command, t_shell *shell)
{
    int flag_builtin;

    shell->pids[shell->index - 1] = fork();
    if (shell->pids[shell->index - 1] < 0)
        return ;
    if (shell->pids[shell->index - 1] == 0)
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

int check_valid_command(t_command *command, t_shell *shell)
{
    // that's for sure not right
    if (!command->command_array[0] || command->command_array[0][0] == '\0')
    {
        ft_putstr_fd("error, command not found\n", 2);
        shell->exitcode = 127;
        return (1);
        // do i need to close pipes here?
    }
    return (0);
}

void    handle_command(t_command *command, t_shell *shell) // this will do in loop
{
    shell->index++;
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

void    command_or_pipe(t_command *command, t_shell *shell)
{
    t_command *current;

    if (!command)
        return ;
    current = command;
    while (current != NULL)
    {
        if (current->type != PIPE)
            handle_command(current, shell);
        current = current->next;
    }
}
*/

int create_pids(t_shell *shell, int command_count)
{
    int i;

    i = 0;
    shell->pids = NULL;
    shell->pids = malloc(command_count * sizeof(int));
    if (!shell->pids)
        return (1);
    while (i < command_count)
    {
        shell->pids[i] = -1;
        i++;
    }
    shell->pids[command_count] = 0;
    return (0);
}

void    shell_execution(t_shell *shell)
{
    int     command_count;
   // printf("Here");

    command_count = shell->data->vec_cmds.len;
    //printf("Here1");
    shell->pipes_count = command_count - 1;
    
    if (create_pids(shell, command_count))
    {
        error_smt();
        return ;
    }
    if (command_count == 1)
    {
        single_command_case(shell);
    }
    /*else
    {
        execute_pipes(shell);

    }
    */
    //cleanup_parent(shell);
    shell->command_index = 0;
    shell->index = 0;
}
