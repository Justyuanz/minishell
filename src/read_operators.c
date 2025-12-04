/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_operators.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:19:14 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/04 15:32:37 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	push_operator_tok(t_data *d, char *line, size_t len, int type)
{
	t_token	*tok;

	tok = (t_token *)arena_alloc(d, &d->arena_tok, sizeof(t_token));
	if (!tok)
		destroy_and_exit(d, "Arena alloc tok fail", 1);
	tok->str = arena_push(&d->arena_tok, line, len + 1);
	if (!tok->str)
		destroy_and_exit(d, "Arena push tok fail", 1);
	tok->str[len] = '\0';
	tok->type = type;
	tok->quote.single_on = false;
	tok->quote.double_on = false;
	if (vec_push(&d->vec_tok, tok) == -1)
		destroy_and_exit(d, "Vec push tok fail\n", 1);
}

size_t	read_pipe(t_data *d, size_t i)
{
	if (d->line[i] == '|')
	{
		push_operator_tok(d, &d->line[i], 1, PIPE);
		i++;
	}
	return (i);
}

size_t	read_redir_operator2(t_data *d, size_t i)
{
	if (d->line[i + 1] == '<')
	{
		push_operator_tok(d, &d->line[i], 2, HEREDOC);
		d->heredoc_skip = 1;
		i += 2;
	}
	else
	{
		push_operator_tok(d, &d->line[i], 1, REDIR_IN);
		i++;
	}
	return (i);
}

size_t	read_redir_operator(t_data *d, size_t i)
{
	if (d->line[i + 1] == '>')
	{
		push_operator_tok(d, &d->line[i], 2, APPEND);
		i += 2;
	}
	else
	{
		push_operator_tok(d, &d->line[i], 1, REDIR_OUT);
		i++;
	}
	return (i);
}
