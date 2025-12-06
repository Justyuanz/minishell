/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:30:10 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/30 16:30:14 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_env_entry(t_env *env)
{
	char	*temp;
	char	*result;

	if (!env || !env->key || !env->value)
		return (NULL);
	temp = ft_strjoin(env->key, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, env->value);
	free(temp);
	return (result);
}

static void	cleanup_partial_envp(char **envp, size_t current_idx)
{
	while (current_idx > 0)
		free(envp[--current_idx]);
	free(envp);
}

static bool	process_env_entry(t_data *data, char **envp, size_t i)
{
	t_env	*env;
	char	*entry;

	env = get_env(data, i);
	entry = create_env_entry(env);
	if (entry)
	{
		envp[i] = entry;
		return (true);
	}
	else
	{
		envp[i] = NULL;
		return (!env || !env->key || !env->value);
	}
}

char	**create_envp_from_data(t_data *data)
{
	char	**envp;
	size_t	i;

	if (!data || data->vec_env.len == 0)
		return (NULL);
	envp = malloc(sizeof(char *) * (data->vec_env.len + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (i < data->vec_env.len)
	{
		if (!process_env_entry(data, envp, i))
		{
			cleanup_partial_envp(envp, i);
			return (NULL);
		}
		i++;
	}
	envp[data->vec_env.len] = NULL;
	return (envp);
}
