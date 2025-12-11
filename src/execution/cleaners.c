/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaners.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:30:04 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/30 16:30:05 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	free_pipes(t_shell *shell)
{
	int	i;

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

void	free_string(char *str)
{
	if (!str)
		return ;
	free(str);
	str = NULL;
}

void	close_parent_pipes(t_shell *shell)
{
	if (shell->index > 0 && shell->pipe_array[shell->index - 1])
		close(shell->pipe_array[shell->index - 1][0]);
	if (shell->index < shell->command_index - 1
		&& shell->pipe_array[shell->index])
		close(shell->pipe_array[shell->index][1]);
}

void	cleanup_env_vars(t_shell *shell)
{
	size_t	i;
	t_env	*env_var;

	i = 0;
	while (i < shell->data->vec_env.len)
	{
		env_var = (t_env *)vec_get(&shell->data->vec_env, i);
		if (env_var)
		{
			if (env_var->key_dupped && env_var->key)
			{
				free(env_var->key);
				env_var->key_dupped = 0;
			}
			if (env_var->value_dupped && env_var->value)
			{
				free(env_var->value);
				env_var->value_dupped = 0;
			}
		}
		i++;
	}
	if (shell && shell->data)
		shell->data->vec_env.len = 0;
}
