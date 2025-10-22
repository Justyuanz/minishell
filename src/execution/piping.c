#include "minishell.h"

void    close_pipes(t_shell *shell)
{
    int i;

    i = 0;
    while (i < shell->command_index - 1)
    {
        if (shell->pipe_array[i])
        {
            if (shell->pipe_array[i][0] > 0)
                close(shell->pipe_array[i][0]);
            if (shell->pipe_array[i][1] > 1)
                close(shell->pipe_array[i][1]);
        }
        i++;
    }
}

void    handle_pipes(t_shell *shell)
{
    if (shell->index == 1)
        if (dup2(shell->pipe_array[shell->index - 1][1], STDOUT_FILENO) == -1)
            error_smt(); // need to think about error handling here
    else if (shell->index > 1 && shell->index < shell->command_index)
    {
        if (dup2(shell->pipe_array[shell->index - 2][0], STDIN_FILENO) == -1)
            error_smt();
        if (dup2(shell->pipe_array[shell->index - 1][1], STDOUT_FILENO) == -1)
            error_smt();
    }
    else if (shell->index == shell->command_index)
    {
        if (dup2(shell->pipe_array[shell->index - 2][0], STDIN_FILENO) == -1)
            error_smt();
    }
    close_pipes(shell);
}

int create_pipes(t_shell *shell)
{
    int i;

    i = 0;
    shell->pipes_count = shell->command_index - 1;
    shell->pipe_array = malloc(shell->pipes_count * sizeof(int *));
    if (!shell->pipe_array)
        return (-1);
    while (i < shell->pipes_count)
    {
        shell->pipe_array[i] = malloc(2 *sizeof(int));
        if (!shell->pipe_array[i])
        {
            free_pipes(shell);
            error_smt(); // some error handling;
            return (-1);
        }
        if (pipe(shell->pipe_array[i]) == -1)
        {
            free_pipes(shell);
            error_smt();
            return (-1);
        }
        i++;
    }
    return (0);
}
