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

char	**create_envp_from_data(t_data *data)
{
	char	**envp;
	size_t	i;
	t_env	*env;
	char	*temp;

	if (!data || data->vec_env.len == 0)
		return (NULL);
	envp = malloc(sizeof(char *) * (data->vec_env.len + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (i < data->vec_env.len)
	{
		env = get_env(data, i);
		if (env && env->key && env->value)
		{
			envp[i] = ft_strjoin(env->key, "=");
			if (envp[i])
			{
				temp = envp[i];
				envp[i] = ft_strjoin(temp, env->value);
				free(temp);
				if (!envp[i])
				{
					while (i > 0)
						free(envp[--i]);
					free(envp);
					return (NULL);
				}
			}
			else
			{
				while (i > 0)
					free(envp[--i]);
				free(envp);
				return (NULL);
			}
		}
		else
			envp[i] = NULL;
		i++;
	}
	envp[data->vec_env.len] = NULL;
	return (envp);
}
