#include "minishell.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i] != NULL)
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}

void    free_pipes(t_shell *shell)
{
    int i;

    i = 0;
    if (!shell->pipe_array)
        return ;
    while (i < shell->pipes_count)
    {
        if (shell->pipe_array[i])
        {
            if (shell->pipe_array[i][0] > 0)
                close(shell->pipe_array[i][0]);
            if (shell->pipe_array[i][1] > 0)
                close(shell->pipe_array[i][1]);
            free(shell->pipe_array[i]);
            shell->pipe_array[i] = NULL;
        }
        i++;
    }
    free(shell->pipe_array);
    shell->pipe_array = NULL;
    shell->index = 0;
}

void    free_string(char *str)
{
    if (!str)
        return ;
    free(str);
    str = NULL;
}