/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:26:48 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/30 16:26:53 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_isspace(char c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ');
}

void	shell_init(t_data *d, t_shell *shell, char **envp)
{
	d->heredoc_skip = 0;
	shell->data = d;
	if (arena_init(&d->arena_tok, 100000) == -1)
		destroy_and_exit(d, "Arena init fail\n", 1);
	if (vec_new(&d->vec_tok, 1, sizeof(t_token *)) == -1)
		destroy_and_exit(d, "Vec init fail\n", 1);
	if (arena_init(&d->arena_env, 100000) == -1)
		destroy_and_exit(d, "Arena init fail\n", 1);
	if (vec_new(&d->vec_env, 1, sizeof(t_env *)) == -1)
		destroy_and_exit(d, "Vec init fail\n", 1);
	if (vec_new(&d->vec_cmds, 1, sizeof(t_cmd *)) == -1)
		destroy_and_exit(d, "Arena init fail\n", 1);
	envp_init(d, envp);
	shell->envp = create_envp_from_data(d); // moved this from top to bottom
}

t_data	*get_data(void)
{
	static t_data	d;

	return (&d);
}

t_shell	*ft_shell(void)
{
	static t_shell	shell;

	return (&shell);
}

bool	str_cmp(char *s1, char *s2)
{
	size_t	i;

	if (!s1 || !s2)
		return (false);
	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (false);
		i++;
	}
	return (true);
}
