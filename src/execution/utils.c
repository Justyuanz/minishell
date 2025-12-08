/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:31:24 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/04 15:41:50 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	if (!s1 || !s2)
		return (0);
	i = 0;
	while (s1[i] == s2[i] && (s1[i] != '\0' || s2[i] != '\0'))
		i++;
	return (s1[i] - s2[i]);
}

void	update_exitcode(int error_code, t_shell *shell)
{
	if (shell->command_index < 2 || shell->index == shell->command_index)
		shell->exitcode = error_code;
}

char	*get_env_value(t_shell *shell, char *str)
{
	t_env	*env_var;
	size_t	i;

	i = 0;
	if (!shell || !shell->data || !str)
		return (NULL);
	while (i < shell->data->vec_env.len)
	{
		env_var = (t_env *)vec_get(&shell->data->vec_env, i);
		if (env_var && env_var->key && ft_strcmp(env_var->key, str) == 0)
			return (env_var->value);
		i++;
	}
	return (NULL);
}

void	final_cleaner(t_shell *shell)
{
	cleanup_line_runtime(shell->data);
	close(shell->savestdout);
	close(shell->savestdin);
	destroy_and_exit(shell->data, NULL, shell->exitcode);
}

void	handle_path_error(const char *cmd, t_shell *shell, int found)
{
	if (found)
	{
		shell->exitcode = 126;
		ft_putstr_fd((char *)cmd, 2);
		ft_putstr_fd(": Permission denied\n", 2);
	}
	else
	{
		shell->exitcode = 127;
		ft_putstr_fd((char *)cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	final_exit(shell, shell->exitcode);
}
