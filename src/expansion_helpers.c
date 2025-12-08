/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:08:24 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/07 22:34:16 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	expand_in_heredoc(t_redir *redir)
{
	return (redir && redir->type == HEREDOC && redir->quote.single_on == false
		&& redir->quote.double_on == false);
}

bool	exit_status(t_data *d, size_t *i, char *buf, size_t *off)
{
	int		code;
	char	*num;
	size_t	k;

	if (d->line[*i] == '?')
	{
		k = 0;
		code = ft_shell()->exitcode;
		num = ft_itoa(code);
		while (num[k])
			buf[(*off)++] = num[k++];
		free(num);
		(*i)++;
		return (true);
	}
	return (false);
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

void	handle_variable(t_data *d, char *buf, size_t *i, size_t *off)
{
	t_env	*env;
	char	tmp[1024];
	size_t	k;
	size_t	j;
	bool	found;

	j = 0;
	found = true;
	store_var_name(d, tmp, i, &j);
	k = ~0;
	j = ~0;
	while (++j < d->vec_env.len)
	{
		env = get_env(d, j);
		if (str_cmp(tmp, env->key) == true)
		{
			found = true;
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
