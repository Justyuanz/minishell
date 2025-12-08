/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:08:24 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/08 11:53:02 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	expand_in_heredoc(t_redir *redir)
{
	return (redir && redir->type == HEREDOC && redir->quote.single_on == false
		&& redir->quote.double_on == false);
}

bool	bare_dollar(t_data *d, char *buf, size_t *off, size_t *i)
{
	if (d->line[*i] == '$')
	{
		buf[*off] = '$';
		(*off)++;
		(*i)++;
		return (true);
	}
	else if (!d->line[*i] || ft_isspace(d->line[*i]) || d->line[*i] == '|'
		|| d->line[*i] == '>' || d->line[*i] == '<' || d->line[*i] == '"')
	{
		buf[*off] = '$';
		(*off)++;
		return (true);
	}
	return (false);
}

void	store_var_name(t_data *d, char *tmp, size_t *i, size_t *j)
{
	while (d->line[*i] && ((d->line[*i] >= 'a' && d->line[*i] <= 'z')
			|| (d->line[*i] >= 'A' && d->line[*i] <= 'Z') || (d->line[*i] >= '0'
				&& d->line[*i] <= '9') || d->line[*i] == '_'))
	{
		if ((*j) >= 1028)
		{
			tmp[*j] = '\0';
			return ;
		}
		tmp[*j] = d->line[*i];
		(*j)++;
		(*i)++;
	}
	tmp[*j] = '\0';
}

static void	copy_env(t_data *d, char *buf, size_t *off, char *tmp)
{
	size_t	j;
	size_t	k;
	t_env	*env;

	k = 0;
	j = ~0;
	while (++j < d->vec_env.len)
	{
		env = get_env(d, j);
		if (str_cmp(tmp, env->key) == true)
		{
			if (env->value[0] == '\0')
				d->expanded_empty = 1;
			while (++k < ft_strlen(env->value) && (*off) < 1023)
			{
				buf[*off] = env->value[k];
				(*off)++;
				if ((*off) >= 1023)
					break ;
			}
			return ;
		}
	}
	d->expanded_empty = 1;
}

void	handle_variable(t_data *d, char *buf, size_t *i, size_t *off)
{
	char	tmp[1024];
	size_t	j;

	j = 0;
	store_var_name(d, tmp, i, &j);
	copy_env(d, buf, off, tmp);
}
