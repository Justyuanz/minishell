/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:29:05 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/30 16:29:07 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_or_add_env_var(t_shell *shell, const char *arg)
{
	char	*equal_sign;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		key = ft_substr(arg, 0, equal_sign - arg);
		value = ft_strdup(equal_sign + 1);
	}
	else
		return (0);
	if (!key)
		return (1);
	if (update_env_var(shell, key, value) == 0)
		return (0);
	if (add_env_var(shell, key, value) == 1)
		return (1);
	return (0);
}

void	process_env_var(t_shell *shell, size_t i)
{
	char	*env_str;
	size_t	total_len;
	t_env	*env_var;

	env_var = (t_env *)vec_get(&shell->data->vec_env, i);
	if (env_var->value != NULL && env_var->key != NULL)
	{
		total_len = ft_strlen(env_var->key) + ft_strlen(env_var->value) + 2;
		env_str = malloc(total_len);
		if (env_str)
		{
			ft_strlcpy(env_str, env_var->key, total_len);
			ft_strlcat(env_str, "=", total_len);
			ft_strlcat(env_str, env_var->value, total_len);
			shell->envp[i] = env_str;
		}
		else
			shell->envp[i] = NULL;
	}
	else
	{
		shell->envp[i] = ft_strdup(env_var->key);
		env_var->key_dupped = 1;
	}
}

void	while_loop_update_envp(t_shell *shell)
{
	size_t	i;

	i = 0;
	while (i < shell->data->vec_env.len)
	{
		process_env_var(shell, i);
		i++;
	}
}

void	update_shell_envp(t_shell *shell)
{
	size_t	i;

	i = 0;
	if (shell->envp)
	{
		i = 0;
		while (shell->envp[i])
			free(shell->envp[i++]);
		free(shell->envp);
	}
	shell->envp = malloc((shell->data->vec_env.len + 1) * sizeof(char *));
	if (!shell->envp)
		return ;
	i = 0;
	while (i <= shell->data->vec_env.len)
		shell->envp[i++] = NULL;
	while_loop_update_envp(shell);
	shell->envp[shell->data->vec_env.len] = NULL;
	shell->exitcode = 0;
}

void	builtin_export(t_cmd *cmd, t_shell *shell)
{
	int	i;

	if (export_argv(cmd, shell) == 1)
		return ;
	i = 0;
	while (cmd->argv[++i])
	{
		if (!is_valid_identifier(cmd->argv[i]))
		{
			ft_putstr_fd("mini:export:not a valid identifier\n", 2);
			shell->exitcode = 1;
		}
		else
		{
			if (update_or_add_env_var(shell, cmd->argv[i]) != 0)
			{
				ft_putstr_fd("export: memory allocation failed\n", 2);
				shell->exitcode = 1;
			}
			else
				update_shell_envp(shell);
		}
	}
}
