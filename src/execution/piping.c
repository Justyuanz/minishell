/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:30:59 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/30 16:31:01 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipes(t_shell *shell)
{
	int	i;

	i = 0;
	while (i < shell->pipes_count)
	{
		if (shell->pipe_array[i])
		{
			//if (shell->pipe_array[i][0] > 0)
				close(shell->pipe_array[i][0]);
			//if (shell->pipe_array[i][1] > 1)
				close(shell->pipe_array[i][1]);
		}
		i++;
	}
}

void	handle_pipes(t_shell *shell)
{
	if (shell->index == 0)
	{
		if (dup2(shell->pipe_array[shell->index][1], STDOUT_FILENO) == -1)
			ft_putstr_fd("dup2 failed for first command", 2);
	}
	else if (shell->index > 0 && shell->index < shell->command_index - 1)
	{
		if (dup2(shell->pipe_array[shell->index - 1][0], STDIN_FILENO) == -1)
			ft_putstr_fd("dup2 failed for middle command input", 2);
		if (dup2(shell->pipe_array[shell->index][1], STDOUT_FILENO) == -1)
			ft_putstr_fd("dup2 failed for middle command output", 2);
	}
	else if (shell->index == shell->command_index - 1)
	{
		if (dup2(shell->pipe_array[shell->index - 1][0], STDIN_FILENO) == -1)
			ft_putstr_fd("dup2 failed for last command", 2);
	}
	close_pipes(shell);
}

int	create_pipes(t_shell *shell)
{
	int	i;

	if (shell->pipes_count <= 0)
		return (0);
	shell->pipe_array = malloc(shell->pipes_count * sizeof(int *));
	if (!shell->pipe_array)
		return (-1);
	i = -1;
	while (++i < shell->pipes_count)
		shell->pipe_array[i] = NULL;
	i = 0;
	while (i < shell->pipes_count)
	{
		shell->pipe_array[i] = malloc(2 * sizeof(int));
		if (!shell->pipe_array[i])
		{
			return (free_pipes(shell), -1);
		}
		if (pipe(shell->pipe_array[i]) == -1)
		{
			return (free_pipes(shell), -1);
		}
		i++;
	}
	return (0);
}
