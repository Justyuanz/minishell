/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:07:13 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/03 23:07:02 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	push_env(t_data *d, char **envp, size_t i)
{
	char	*str;
	char	*equal;
	t_env	*env;

	str = arena_push(&d->arena_env, envp[i], ft_strlen(envp[i]) + 1);
	if (!str)
		destroy_and_exit(d, "Arena push env fail", 1);
	equal = ft_strchr(str, '=');
	if (equal)
	{
		*equal = '\0';
		env = (t_env *)arena_alloc(d, &d->arena_env, sizeof(t_env));
		if (!env)
			destroy_and_exit(d, "Arena alloc env fail", 1);
		env->key = str;
		env->value = equal + 1;
		env->key_dupped = 0;
		env->value_dupped = 0;
		if (vec_push(&d->vec_env, env) == -1)
			destroy_and_exit(d, "Vec push env fail", 1);
	}
}

void	envp_init(t_data *d, char **envp)
{
	size_t	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		push_env(d, envp, i);
		i++;
	}
}
