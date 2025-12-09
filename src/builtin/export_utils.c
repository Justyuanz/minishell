/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_others.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:29:15 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/04 19:15:46 by jinzhang         ###   ########.fr       */
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

int	update_env_var(t_shell *shell, char *key, char *value)
{
	size_t	i;
	t_env	*env_var;

	i = 0;
	while (i < shell->data->vec_env.len)
	{
		env_var = (t_env *)vec_get(&shell->data->vec_env, i);
		if (env_var && ft_strcmp(env_var->key, key) == 0)
		{
			if (env_var->value && env_var->value_dupped)
				free(env_var->value);
			env_var->value = value;
			env_var->value_dupped = 1;
			free(key);
			return (0);
		}
		i++;
	}
	return (1);
}

int	add_env_var(t_shell *shell, char *key, char *value)
{
	t_env	*env_var;

	env_var = arena_alloc(shell->data, &shell->data->arena_env, sizeof(t_env));
	if (!env_var)
	{
		free(key);
		if (value)
			free(value);
		return (1);
	}
	env_var->key = key;
	env_var->value = value;
	env_var->key_dupped = 1;
	env_var->value_dupped = 1;
	if (!vec_push(&shell->data->vec_env, env_var))
	{
		free(key);
		if (value)
			free(value);
		return (1);
	}
	return (0);
}
