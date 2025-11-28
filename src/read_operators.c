/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_operators.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:19:14 by jinzhang          #+#    #+#             */
/*   Updated: 2025/11/28 20:19:18 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	read_pipe(t_data *d, size_t i, t_quote quote)
{
	if (d->line[i] == '|')
	{
		push_tok(d, &d->line[i], 1, PIPE, quote);
		i++;
	}
	return (i);
}

size_t	read_redir_operator2(t_data *d, size_t i, t_quote quote)
{
	if (d->line[i + 1] == '<')
	{
		push_tok(d, &d->line[i], 2, HEREDOC, quote);
		d->heredoc_skip = 1;
		i += 2;
	}
	else
	{
		push_tok(d, &d->line[i], 1, REDIR_IN, quote);
		i++;
	}
	return (i);
}

size_t	read_redir_operator(t_data *d, size_t i, t_quote quote)
{
	if (d->line[i + 1] == '>')
	{
		push_tok(d, &d->line[i], 2, APPEND, quote);
		i += 2;
	}
	else
	{
		push_tok(d, &d->line[i], 1, REDIR_OUT, quote);
		i++;
	}
	return (i);
}
