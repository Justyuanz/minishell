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

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	print_exported_vars(t_shell *shell)
{
	t_env	*env_var;
	size_t	i;

	i = 0;
	while (i < shell->data->vec_env.len)
	{
		env_var = (t_env *)vec_get(&shell->data->vec_env, i);
		if (env_var && env_var->key)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(env_var->key, STDOUT_FILENO);
			if (env_var->value)
			{
				ft_putstr_fd("=\"", STDOUT_FILENO);
				ft_putstr_fd(env_var->value, STDOUT_FILENO);
				ft_putstr_fd("\"", STDOUT_FILENO);
			}
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		i++;
	}
}

int	update_or_add_env_var(t_shell *shell, const char *arg)
{
	char	*equal_sign;
	char	*key;
	char	*value;
	t_env	*env_var;
	size_t	i;

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
	i = 0;
	while (i < shell->data->vec_env.len)
	{
		env_var = (t_env *)vec_get(&shell->data->vec_env, i);
		if (env_var && ft_strcmp(env_var->key, key) == 0)
		{
			env_var->value = value;
			free(key);
			return (0);
		}
		i++;
	}
	env_var = malloc(sizeof(t_env));
	if (!env_var)
	{
		free(key);
		free(value);
		return (1);
	}
	env_var->key = key;
	env_var->value = value;
	if (!vec_push(&shell->data->vec_env, env_var))
	{
		free(key);
		free(value);
		free(env_var);
		return (1);
	}
	return (0);
}

void	update_shell_envp(t_shell *shell)
{
	size_t	i;
	t_env	*env_var;
	char	*env_str;

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
	while (i < shell->data->vec_env.len)
	{
		env_var = (t_env *)vec_get(&shell->data->vec_env, i);
		if (env_var->value)
		{
			env_str = malloc(ft_strlen(env_var->key) + ft_strlen(env_var->value)
					+ 2);
			if (env_str)
			{
				ft_strlcpy(env_str, env_var->key, ft_strlen(env_var->key) + 1);
				ft_strlcat(env_str, "=", ft_strlen(env_var->key) + 2);
				ft_strlcat(env_str, env_var->value, ft_strlen(env_var->key)
					+ ft_strlen(env_var->value) + 2);
			}
			shell->envp[i] = env_str;
		}
		else
			shell->envp[i] = ft_strdup(env_var->key);
		i++;
	}
	shell->envp[i] = NULL;
}

void	builtin_export(t_cmd *cmd, t_shell *shell)
{
	int	i;

	if (!cmd->argv[1])
	{
		print_exported_vars(shell);
		shell->exitcode = 0;
		return ;
	}
	i = 1;
	while (cmd->argv[i])
	{
		if (!is_valid_identifier(cmd->argv[i]))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(cmd->argv[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			shell->exitcode = 1;
		}
		else
		{
			if (update_or_add_env_var(shell, cmd->argv[i]) != 0)
			{
				ft_putstr_fd("minishell: export: memory allocation failed\n",
					STDERR_FILENO);
				shell->exitcode = 1;
			}
			else
				update_shell_envp(shell);
		}
		i++;
	}
}
