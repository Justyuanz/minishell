/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:06:00 by jinzhang          #+#    #+#             */
/*   Updated: 2025/12/08 19:51:57 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	check_start_or_end_pipe(t_data *d)
{
	if (get_tok(d, 0)->type == PIPE || get_tok(d, (d->vec_tok.len
				- 1))->type == PIPE)
		return (parse_error_msg(SYNTAX_ERROR_PIPE, NULL, 2));
	return (true);
}

static bool	check_end_with_redirs(t_data *d, size_t i)
{
	t_token	*tok;

	tok = get_tok(d, i);
	if ((i + 1 < d->vec_tok.len && get_tok(d, i + 1)->type != EMPTY_WORD))
		return (true);
	else if ((tok->type == REDIR_IN || tok->type == REDIR_OUT
			|| tok->type == APPEND || tok->type == HEREDOC))
		return (parse_error_msg(SYNTAX_ERROR_NEWLINE, NULL, 2));
	return (true);
}

static bool	check_redir_sequence(t_data *d)
{
	t_token	*tok;
	t_token	*next_tok;
	size_t	i;

	i = 0;
	while (i < d->vec_tok.len - 1)
	{
		tok = get_tok(d, i);
		next_tok = get_tok(d, i + 1);
		if (tok->type == REDIR_IN || tok->type == REDIR_OUT
			|| tok->type == APPEND || tok->type == HEREDOC)
			if (!(next_tok->type == WORD || next_tok->type == EMPTY_WORD))
				return (parse_error_msg(SYNTAX_ERROR_REDIR, next_tok->str, 2));
		i++;
	}
	if (!check_end_with_redirs(d, i))
		return (false);
	return (true);
}

static bool	check_pipe_sequence(t_data *d)
{
	t_token	*tok;
	t_token	*next_tok;
	size_t	i;

	i = 0;
	while (i < d->vec_tok.len - 1)
	{
		tok = get_tok(d, i);
		next_tok = get_tok(d, i + 1);
		if (tok->type == PIPE)
		{
			if (next_tok->type == PIPE)
				return (parse_error_msg(SYNTAX_ERROR_PIPE, NULL, 2));
		}
		i++;
	}
	return (true);
}

bool	syntax_validation(t_data *d)
{
	if (d->vec_tok.len == 0)
		return (true);
	if (d->unclosed_quote == 1)
		return (parse_error_msg(SYNTAX_ERROR_QUOTE, NULL, 2));
	if (!check_start_or_end_pipe(d) || !check_redir_sequence(d)
		|| !check_pipe_sequence(d))
		return (false);
	return (true);
}
